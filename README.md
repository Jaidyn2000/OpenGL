# OpenGL
Game Engine (In Progress)

to compile through Visual Studio:
1. Go to Project Properties > C/C++ > Additional Include Directories: $(SolutionDir)Dependencies\bin
2. Go to Project Properties > Linker > Input: opengl32.lib;glfw3.lib;glew32s.lib;%(AdditionalDependencies)
3. Go to Project Properties > Linker > General > Additional Library Directories: $(SolutionDir)Dependencies\lib
