project "Strvy-Editor"
    location "Strvy-Editor"
    kind "ConsoleApp"
    language "C++"
    location "."
    cppdialect "C++17"
    staticruntime "off"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/strvy/vendor/spdlog/include",
        "%{wks.location}/strvy/src",
        "%{IncludeDir.glm}",
        "%{wks.location}/strvy/vendor",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.Assimp}"
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