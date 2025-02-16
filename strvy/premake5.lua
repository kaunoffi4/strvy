project "strvy"
    location "strvy"
    kind "StaticLib"
    language "C++"
    location "."
    cppdialect "C++17"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "svpch.h"
    pchsource "src/svpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{wks.location}/Strvy-Editor",
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
        "yaml-cpp",
        "opengl32.lib"
    }

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "_CRT_SECURE_NO_WARNINGS",
        "YAML_CPP_STATIC_DEFINE"
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