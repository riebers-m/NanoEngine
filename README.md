# Custom Game Engine
This engine is based on the simple 2D game engine course on Pikuma.

## Dependencies
* SDL2
* GLM
* LUA
* SOL3
* DearImGui

### Setup SDL2
#### Windows MSVC Compiler
1. Download a SDL2-VC.zip Release: https://github.com/libsdl-org/SDL/releases
2. Download a SDL2_image-VC.zip Release: https://github.com/libsdl-org/SDL_image/releases
2. Unzip the folder
3. Set CMAKE_PREFIX_PATH to the SDL2 cmake, the path to the SDL2_image.dll:
````cmake
cmake -B build -DCMAKE_PREFIX_PATH="path/to/SDL2/cmake;path/to/SDL2_image/cmake;path/to/SDL2_ttf/cmake" -DSDL2_IMAGE_DLL_DIR="path/to/SDL2_image.dll"  
````
