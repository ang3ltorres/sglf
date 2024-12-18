cmake_minimum_required(VERSION 3.29)
project(sglf LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES_SGLF CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/sglf/*.cpp")
add_library(sglf ${SOURCES_SGLF})
target_include_directories(sglf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/sglf")
target_compile_options(sglf PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_compile_definitions(sglf PRIVATE WIN32_LEAN_AND_MEAN)
target_link_libraries(sglf PUBLIC plutosvg plutovg Vorbis::vorbisfile Vorbis::vorbis Ogg::ogg glm::glm-header-only gdiplus dsound windowscodecs opengl32 user32 gdi32 user32 ole32)

file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp")
add_executable(test ${SOURCES_TEST})
target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/test" "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(sglf PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_compile_definitions(sglf PRIVATE WIN32_LEAN_AND_MEAN)
target_link_libraries(test PRIVATE sglf)
