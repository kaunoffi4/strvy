workspace "strvy"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "strvy/vendor/GLFW/include"

include "strvy/vendor/GLFW"

project "strvy"
    location "strvy"
    kind "SharedLib"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "svpch.h"
    pchsource "strvy/src/svpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "SV_PLATFORM_WINDOWS",
            "SV_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
        }

        filter "configurations:Debug"
            defines "SV_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "SV_RELEASE"
            optimize "On"

        filter "configurations:Dist"
            defines "SV_DIST"
            optimize "On"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "strvy/vendor/spdlog/include",
        "strvy/src"
    }

    links
    {
        "strvy"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {
            "SV_PLATFORM_WINDOWS"
        }


        filter "configurations:Debug"
            defines "SV_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "SV_RELEASE"
            optimize "On"

        filter "configurations:Dist"
            defines "SV_DIST"
            optimize "On"

