cmake_minimum_required(VERSION 3.26)
project(AlexZorn_CHIP8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "D:/Libraries/SDL2/SDL2-2.28.2-mingw/x86_64-w64-mingw32")

include_directories("D:/EPTM/Modules/PR_PROJ/PRO/AlexZorn_CHIP8/src/Display")
include_directories("D:/EPTM/Modules/PR_PROJ/PRO/AlexZorn_CHIP8/src/Keyboard")
include_directories("D:/EPTM/Modules/PR_PROJ/PRO/AlexZorn_CHIP8/src/cpu")
include_directories("D:/EPTM/Modules/PR_PROJ/PRO/AlexZorn_CHIP8/src/Stack")


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})


add_executable(AlexZorn_CHIP8 main.c
        src/Display/display.c
        src/Keyboard/keyboard.c
        src/cpu/cpu.c
        src/Stack/stack.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})