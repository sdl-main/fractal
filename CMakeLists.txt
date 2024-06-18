cmake_minimum_required(VERSION 2.8)
project(fractal)

set(SOURCE src/main.cpp)
add_subdirectory("lib/glfw")
add_subdirectory("lib/glew-2.1.0/build/cmake")
set(GLEW_DIR "lib/glew-2.1.0/build/cmake")
add_executable(fractal ${SOURCE})

find_package(OpenGL REQUIRED)
target_include_directories(fractal PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(fractal ${OPENGL_gl_LIBRARY})
target_link_libraries(fractal ${OPENGL_glu_LIBRARY})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
target_include_directories(fractal PUBLIC "lib/glfw/include")
target_include_directories(fractal PUBLIC "lib/glew-2.1.0/include")
find_package(GLEW 2.1.0 REQUIRED)
link_directories(lib/Simple-OpenGL-Image-Library)
target_link_libraries(fractal glfw GLEW::GLEW SOIL)

