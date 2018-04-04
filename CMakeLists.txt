cmake_minimum_required (VERSION 2.6)
project (n_dims)

find_package(OpenGL REQUIRED)

include_directories(common/)

# To successfully run this you need to be in the common folder so it can 
# can find the shaders. Run the following commands to make and run it all
# mkdir build
# cd build
# cmake ..
# make
# cd ../common
# ../build/test_program
add_executable(test_program
    src/test_program.cpp
    common/shader.cpp
    common/shader.hpp

    common/Vertex.glsl
    common/Fragment.glsl
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW
)

target_link_libraries(test_program
    ${ALL_LIBS}
)
