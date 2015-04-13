# Global project settings
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(irma)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${irma_SOURCE_DIR}/cmake")

# lua (Library)
aux_source_directory(lua LUA_SRC)
add_library(lua ${LUA_SRC})

# irma (Player)
aux_source_directory(src IRMA_SRC)
add_executable(irma ${IRMA_SRC})
target_link_libraries(irma lua)

target_compile_features(irma PRIVATE cxx_range_for)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(irma ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories($(GLEW_INCLUDE_DIRS))
target_link_libraries(irma ${GLEW_LIBRARIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(irma ${SDL2_LIBRARY})
