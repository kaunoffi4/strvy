include "Dependencies.lua"

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


group "Dependencies"
    include "vendor/premake"
    include "strvy/vendor/GLFW"
    include "strvy/vendor/Glad"
    include "strvy/vendor/imgui"
    include "strvy/vendor/yaml-cpp"
group ""

include "strvy"
include "Game"
include "Strvy-Editor"

