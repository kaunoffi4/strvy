workspace "strvy"
    architecture "x64"
    startproject "Strvy-Editor"

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
IncludeDir["stb_image"] = "strvy/vendor/stb_image"
IncludeDir["entt"] = "strvy/vendor/entt/include"
IncludeDir["yaml_cpp"] = "strvy/vendor/yaml-cpp/include"


group "Dependencies"
    include "strvy/vendor/GLFW"
    include "strvy/vendor/Glad"
    include "strvy/vendor/imgui"
    include "strvy/vendor/yaml-cpp"




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
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"

    }

    links
    {
        "Glad",
        "GLFW",
        "ImGui",
        "opengl32.lib",
        "yaml-cpp"
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
        "strvy/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "Game"
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



project "Strvy-Editor"
    location "Strvy-Editor"
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
        "%{IncludeDir.glm}",
        "strvy/vendor",
        "%{IncludeDir.entt}",
        "Game"
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