workspace "strvy"
    architecture "x64"
    startproject "Game"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "strvy/vendor/GLFW/include"
IncludeDir["Glad"] = "strvy/vendor/Glad/include"
IncludeDir["ImGui"] = "strvy/vendor/imgui"
IncludeDir["glm"] = "strvy/vendor/glm"



include "strvy/vendor/GLFW"
include "strvy/vendor/Glad"
include "strvy/vendor/imgui"




project "strvy"
    location "strvy"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "svpch.h"
    pchsource "strvy/src/svpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Glad",
        "GLFW",
        "ImGui",
        "opengl32.lib"
    }

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    filter "system:windows"
        systemversion "latest"
        

        defines 
        {
            "SV_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

        filter "configurations:Debug"
            defines "SV_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "SV_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "SV_DIST"
            runtime "Release"
            optimize "on"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "strvy/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "strvy"
    }

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"
    }
    filter "system:windows"
        systemversion "latest"

        defines 
        {
            "SV_PLATFORM_WINDOWS"
        }


        filter "configurations:Debug"
            defines "SV_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "SV_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "SV_DIST"
            runtime "Release"
            optimize "on"

