import sys
import os
import subprocess
from pathlib import Path

import Utils

class AssimpConfiguration:
    assimpVersion = "5.4.3"
    assimpZipUrls = f"https://github.com/assimp/assimp/archive/refs/tags/v{assimpVersion}.zip"
    assimpDirectory = f"./strvy/vendor/assimp"
    assimpVersionDirectory = f"{assimpDirectory}/assimp-{assimpVersion}/"
    buildDirectory = os.path.join(assimpVersionDirectory , "build")
    installDirectory = os.path.join(assimpVersionDirectory , "install")

    
    @classmethod
    def Validate(cls):
        if (not cls.CheckIfAssimpInstalled()):
            print("Assimp is not installed.")
            return False

        print(f"Correct Assimp located at {os.path.abspath(cls.assimpDirectory)}")

        file_to_rename = f"{cls.assimpVersionDirectory }/INSTALL"
        file_renamed = f"{cls.assimpVersionDirectory }/INSTALL.txt"
        
        if os.path.exists(file_to_rename) and not os.path.exists(file_renamed):
            os.rename(file_to_rename, file_renamed)
            print(f"Renamed {file_to_rename} to {file_renamed}")
        else:
            print("File doesn't exist")

        cls.ConfigureCMake()
        cls.BuildAssimp("Debug")
        cls.BuildAssimp("Release")

        return True
        
    @classmethod
    def CheckIfAssimpInstalled(cls):
        assimpFolder = Path(f"{cls.assimpDirectory}")
        if (not assimpFolder.is_dir()):
            return cls.InstallAssimp()

        return True
        
    @classmethod
    def InstallAssimp(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input("Assimp not found. Would you like to download Assimp {0:s}? [Y/N]: ".format(cls.assimpVersion))).lower().strip()[:1]
            if reply == 'n':
                return False
            permissionGranted = (reply == 'y')

        assimpPath = f"{cls.assimpDirectory}/assimp-{cls.assimpVersion}.zip"
        print("Downloading {0:s} to {1:s}".format(cls.assimpZipUrls, assimpPath))
        Utils.DownloadFile(cls.assimpZipUrls, assimpPath)
        print("Extracting", assimpPath)
        Utils.UnzipFile(assimpPath, deleteZipFile=True)
        print(f"assimp {cls.assimpVersion} has been downloaded to '{cls.assimpDirectory}'")

        return True


    @classmethod
    def ConfigureCMake(cls):
        buildFolder = Path(f"{cls.buildDirectory}")
        if not buildFolder.is_dir():
            buildFolder.mkdir()

        cmake_command = [
            "cmake",
            f"-S{cls.assimpDirectory}/assimp-{cls.assimpVersion}",
            f"-B{cls.buildDirectory}",
            "-G", "Visual Studio 17 2022",
            f"-DCMAKE_INSTALL_PREFIX={cls.installDirectory}"
        ]
        
        print(f"Running CMake configuration...\n{' '.join(cmake_command)}")
        result = subprocess.run(cmake_command, shell=True)
        if result.returncode != 0:
            print("CMake configuration failed.")
            return False
        print("CMake configuration complete.")
        return True

    @classmethod
    def BuildAssimp(cls, config="Release"):
        buildFolder = Path(f"{cls.buildDirectory}")
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
        result = subprocess.run(cmake_command, shell=True)
        if result.returncode != 0:
            print("Build failed.")
            return False
        print(f"Assimp {config} build complete.")
        return True
