###irma

The **I**nteractive **R**ay**MA**rcher is an experiment for creating interactive raymarching applications by
combining 2-triangle (Shadertoy-style) GLSL shaders for rendering and LUA scripting for interaction logic.

### How to compile

irma uses [CMake](http://www.cmake.org/) for finding all the dependencies and generating the makefiles/project files for your platform of choice.
Development happens mainly on Visual Studio, so there are some tweaks for generating optimal .sln files.

Although unusual, the minimal CMake version is 3.1, and the compiler must support C++11 in order to compile irma.

CMake will try to find the following dependencies needed to build irma:

  - [SDL 2](https://www.libsdl.org/) (and OpenGL)
  - [GLEW](http://glew.sourceforge.net/)
  - [LuaJIT](http://luajit.org/)
