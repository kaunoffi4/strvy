# strvy
2D basic engine with support for extension to 3D rendering due to the modularity of the project architecture.


# How to set up 
1. Downloading the repository:
Start by cloning the repository with `git clone --recursive https://github.com/kaunoffi4/strvy` .

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

2. Execute `Win-GenProjects.bat` script file, which will then generate a Visual Studio solution file for user's usage.


# Overview

Now I'm focusing on implementing an efficient and working 2D renderer but the actual aim of the project is to become a solid 3D real-time renderer / engine with more advanced features such as lighting calculations, post effects or procedural terrain generation. 

We use the GLFW to create a window and to make the OpenGL context 

hooked up the OpenGL (Glad) to load all of the modern OpenGL functions from the GPU drivers into our C++ code



