cmake_minimum_required(VERSION 3.29)
project(sglf LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(sglf ${SOURCES})
target_include_directories(sglf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(sglf PRIVATE -Wall -pedantic -Wno-narrowing -g -O0 -fmax-errors=1)
target_compile_definitions(sglf PRIVATE WIN32_LEAN_AND_MEAN)
target_link_libraries(sglf PRIVATE Vorbis::vorbisfile Vorbis::vorbis Ogg::ogg glm::glm-header-only dsound windowscodecs opengl32 user32 gdi32 user32 ole32)
