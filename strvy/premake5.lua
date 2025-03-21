project "strvy"
    location "strvy"
    kind "StaticLib"
    language "C++"
    location "."
    cppdialect "C++17"
    staticruntime "off"

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
        "vendor/glm/glm/**.inl",

        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp"
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
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.Assimp}"
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
    
    filter "files:vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }

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

		    links
		    {
			    "%{Library.ShaderC_Debug}",
			    "%{Library.SPIRV_Cross_Debug}",
			    "%{Library.SPIRV_Cross_GLSL_Debug}",
                "%{Library.Assimp_Debug}"
		    }

	    filter "configurations:Release"
		    defines "SV_RELEASE"
		    runtime "Release"
		    optimize "on"

		    links
		    {
			    "%{Library.ShaderC_Release}",
			    "%{Library.SPIRV_Cross_Release}",
			    "%{Library.SPIRV_Cross_GLSL_Release}",
                "%{Library.Assimp_Release}"
		    }

	    filter "configurations:Dist"
		    defines "SV_DIST"
		    runtime "Release"
		    optimize "on"

		    links
		    {
			    "%{Library.ShaderC_Release}",
			    "%{Library.SPIRV_Cross_Release}",
			    "%{Library.SPIRV_Cross_GLSL_Release}",
                "%{Library.Assimp_Release}"
		    }