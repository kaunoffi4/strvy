workspace "strvy"
    architecture "x86_64"
    startproject "Strvy-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/strvy/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/strvy/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/strvy/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/strvy/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/strvy/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/strvy/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/strvy/vendor/yaml-cpp/include"


group "Dependencies"
    include "strvy/vendor/GLFW"
    include "strvy/vendor/Glad"
    include "strvy/vendor/imgui"
    include "strvy/vendor/yaml-cpp"
group ""

include "strvy"
include "Game"
include "Strvy-Editor"

