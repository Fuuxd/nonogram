cmake_minimum_required(VERSION 3.5.0)
project(Nonogram VERSION 1.0.0)

file(GLOB IMGUI_SRC "src/imgui/*.cpp")

add_executable(Nonogram 
                src/config.h 
                src/main.cpp 
                src/glad.c 
                src/nonogram.h
                src/nonogram.cpp
                src/nonoIcon.h
                src/benchmarking.h
                src/permutationVector.h
                ${IMGUI_SRC}
                src/resource.rc
)

target_include_directories(Nonogram PRIVATE dependencies)



set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(./glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(Nonogram -static glfw OpenGL::GL)
target_link_libraries(Nonogram -static OpenGL::GL)

#O2 Optmization
target_compile_options(Nonogram PRIVATE -O2)