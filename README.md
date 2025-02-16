# strvy
Basic 2D engine with support for extension to 3D rendering due to the modularity of the project architecture.


# How to set up 
Downloading the repository:
Start by cloning the repository with `git clone --recursive https://github.com/kaunoffi4/strvy` .

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

Execute `Win-GenProjects.bat` script file, which will then generate a Visual Studio solution file for user's usage.

If changes are made, or if you want to regenerate project files, rerun the Win-GenProjects.bat script file found in scripts folder.

At this point if you did previous steps, you should be able to run and see a window, with some rendering and UI.


# Overview

Now I'm focusing on implementing an efficient and working 2D renderer but the actual aim of the project is to become a solid 3D real-time renderer / engine with more advanced features such as lighting calculations, post effects or procedural terrain generation. 

We use GLFW to create a window and to make OpenGL context 

hooked up the OpenGL (Glad) to load all of the modern OpenGL functions from the GPU drivers into our C++ code

integrated ImGui to enable interacting with variables in real-time manner, to see the changes we made.  



