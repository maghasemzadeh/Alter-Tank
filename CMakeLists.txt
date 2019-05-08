cmake_minimum_required(VERSION 3.0)
project(project C)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCE "src/*.c" "src/*.h")
add_executable(project "${SOURCE}" src/init.c src/init.h src/effects.c src/effects.h src/stack.c src/stack.h src/AI.c src/AI.h src/UI.c src/UI.h src/power-ups.c src/power-ups.h)

include_directories("/usr/include/SDL2")
target_link_libraries(project m SDL2 SDL2_gfx)
