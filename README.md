# strvy
Basic 2D engine with support for 3D rendering extension due to the modularity of the project architecture.


# How to set up 
Downloading the repository:
Start by cloning the repository with `git clone --recursive https://github.com/kaunoffi4/strvy` .

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

Execute `Win-GenProjects.bat` script file, which will then generate a Visual Studio solution file for user's usage.

If changes are made, or if you want to regenerate project files, rerun the Win-GenProjects.bat script file found in scripts folder.

At this point if you did previous steps, you should be able to run and see a window, with UI, try to create and render some objects using RMB click on the left panel. To rotate a camera hold left alt while pressing LMB and move the mouse.


# Overview

Now I'm focusing on implementing an efficient and working 2D renderer but the actual aim of the project is to gain a solid 3D real-time renderer / engine with more advanced features such as lighting calculations, post effects or procedural terrain generation. 

I use GLFW for windows creation and to establish OpenGL context 

Also Glad (OpenGL functions' pointers ) has been hooked up to load all of the modern OpenGL functions from the GPU drivers into C++ code

integrated ImGui to enable interaction with variables in a real-time manner, to see changes being made.  



