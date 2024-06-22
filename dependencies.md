Ideally the user does not need to build it themselves, nevertheless all libraries and dependencies should be contained in the folder itself, there is no need for either the builder or the user to have to deal with installing these

Additionally all of this is built for x64 Windows and only confirmed working in it (Specifically Windows 10 Home 22H2 19045.4412, with OpenGL Version: 4.6.0 NVIDIA 552.44). Changing the glfw and glad files to corresponding OS's files along with disabling benchmarking should make it work. 

Personal system is running 

##Dependencies
---
opengl
glfw3
glad
khr

###Benchmarking, Windows only:
---
windows.h
psapi.h