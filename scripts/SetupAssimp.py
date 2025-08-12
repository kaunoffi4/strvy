#!/usr/bin/env python3
import sys
import os
import subprocess
import shutil
from pathlib import Path
from datetime import datetime

import Utils

class AssimpConfiguration:
    assimpVersion = "5.4.3"
    assimpZipUrls = f"https://github.com/assimp/assimp/archive/refs/tags/v{assimpVersion}.zip"
    assimpDirectory = f"./strvy/vendor/assimp"
    assimpVersionDirectory = f"{assimpDirectory}/assimp-{assimpVersion}"
    buildDirectory = os.path.join(assimpVersionDirectory, "build")
    installDirectory = os.path.join(assimpVersionDirectory, "install")
    installBinDirectory = os.path.join(installDirectory, "bin")

    # Map config -> expected DLL name
    dll_for_config = {
        "Release": "assimp-vc143-mt.dll",
        "Debug":   "assimp-vc143-mtd.dll"
    }

    # Map config -> destination path (bin/<folder>/<Strvy-Editor>)
    dest_for_config = {
        "Release": Path(f"./bin") / "Release-windows-x86_64" / "Strvy-Editor",
        "Debug":   Path(f"./bin") / "Debug-windows-x86_64" / "Strvy-Editor",
    }

    @classmethod
    def Validate(cls):
        if not cls.CheckIfAssimpInstalled():
            print("Assimp is not installed.")
            return False

        print(f"Correct Assimp located at {os.path.abspath(cls.assimpDirectory)}")

        file_to_rename = f"{cls.assimpVersionDirectory}/INSTALL"
        file_renamed = f"{cls.assimpVersionDirectory}/INSTALL.txt"

        if os.path.exists(file_to_rename) and not os.path.exists(file_renamed):
            os.rename(file_to_rename, file_renamed)
            print(f"Renamed {file_to_rename} to {file_renamed}")
        else:
            if os.path.exists(file_to_rename):
                print(f"{file_to_rename} already renamed.")
            else:
                print("INSTALL file doesn't exist")

        if not cls.ConfigureCMake():
            return False

        # Build Debug and Release, and move DLL after each successful install
        if not cls.BuildAssimp("Debug"):
            return False
        if not cls.BuildAssimp("Release"):
            return False

        return True

    @classmethod
    def CheckIfAssimpInstalled(cls):
        assimpFolder = Path(cls.assimpDirectory)
        if not assimpFolder.is_dir():
            return cls.InstallAssimp()
        return True

    @classmethod
    def InstallAssimp(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input(f"Assimp not found. Would you like to download Assimp {cls.assimpVersion}? [Y/N]: ")).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        assimpPath = f"{cls.assimpDirectory}/assimp-{cls.assimpVersion}.zip"
        print(f"Downloading {cls.assimpZipUrls} to {assimpPath}")
        Utils.DownloadFile(cls.assimpZipUrls, assimpPath)
        print("Extracting", assimpPath)
        Utils.UnzipFile(assimpPath, deleteZipFile=True)
        print(f"assimp {cls.assimpVersion} has been downloaded to '{cls.assimpDirectory}'")

        return True

    @classmethod
    def ConfigureCMake(cls):
        buildFolder = Path(cls.buildDirectory)
        buildFolder.mkdir(parents=True, exist_ok=True)

        cmake_command = [
            "cmake",
            f"-S{cls.assimpDirectory}/assimp-{cls.assimpVersion}",
            f"-B{cls.buildDirectory}",
            "-G", "Visual Studio 17 2022",
            f"-DCMAKE_INSTALL_PREFIX={cls.installDirectory}"
        ]

        print(f"Running CMake configuration...\n{' '.join(cmake_command)}")
        try:
            subprocess.run(cmake_command, check=True)
        except subprocess.CalledProcessError:
            print("CMake configuration failed.")
            return False
        print("CMake configuration complete.")
        return True

    @classmethod
    def BuildAssimp(cls, config="Release"):
        buildFolder = Path(cls.buildDirectory)
        if not buildFolder.is_dir():
            print("Error: Build directory does not exist.")
            return False

        cmake_command = [
            "cmake",
            "--build", str(cls.buildDirectory),
            "--config", config,
            "--target", "install"
        ]

        print(f"Building Assimp in {config} mode...\n{' '.join(cmake_command)}")
        try:
            subprocess.run(cmake_command, check=True)
        except subprocess.CalledProcessError:
            print("Build failed.")
            return False

        print(f"Assimp {config} build complete. Attempting to move DLL(s) for {config}...")
        moved = cls._move_dll_for_config(config=config, overwrite=False)
        if not moved:
            print(f"Warning: Could not find or move the assimp DLL for config {config}.")
        return True

    @classmethod
    def _find_file_anywhere(cls, root: Path, name: str):
        """Return first match Path or None."""
        if not root.exists():
            return None
        matches = list(root.rglob(name))
        return matches[0] if matches else None

    @classmethod
    def _backup_if_exists(cls, dest_file: Path):
        if dest_file.exists():
            ts = datetime.now().strftime("%Y%m%d-%H%M%S")
            backup = dest_file.with_name(f"{dest_file.stem}.backup-{ts}{dest_file.suffix}")
            dest_file.replace(backup)
            print(f"Backed up existing {dest_file} -> {backup}")
            return backup
        return None

    @classmethod
    def _move_dll_for_config(cls, config="Release", overwrite=False):
        """
        Move the DLL associated with `config` into the configured destination folder.
        Returns True if the DLL was found and moved, False otherwise.
        """
        dll_name = cls.dll_for_config.get(config)
        if not dll_name:
            print(f"No DLL mapping for config '{config}'")
            return False

        # Try installDirectory first, then buildDirectory
        install_root = Path(cls.installBinDirectory)

        dll_path = cls._find_file_anywhere(install_root, dll_name)

        if dll_path is None:
            print(f"Could not locate {dll_name} in install or build directories.")
            return False

        dest_dir = cls.dest_for_config.get(config)
        if dest_dir is None:
            print(f"No destination configured for {config}")
            return False

        dest_dir = dest_dir.resolve()
        dest_dir.mkdir(parents=True, exist_ok=True)
        dest_file = dest_dir / dll_name

        try:
            if dest_file.exists():
                if overwrite:
                    dest_file.unlink()
                    print(f"Overwriting existing file at {dest_file}")
                else:
                    cls._backup_if_exists(dest_file)

            # Move the DLL (actual move). Change to shutil.copy2 for copy instead.
            shutil.move(str(dll_path), str(dest_file))
            print(f"Moved DLL: {dll_path} -> {dest_file}")
            return True
        except PermissionError as e:
            print(f"Permission error while moving DLL: {e}")
            return False
        except Exception as e:
            print(f"Unexpected error while moving DLL: {e}")
            return False


if __name__ == "__main__":
    ok = AssimpConfiguration.Validate()
    sys.exit(0 if ok else 1)
