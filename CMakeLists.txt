cmake_minimum_required(VERSION 3.29)
project(sglf VERSION 0.6.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
file(GLOB_RECURSE SOURCES_SGLF CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Define the library
add_executable(sglf ${SOURCES_SGLF})

# Include directories
target_include_directories(sglf PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Precompiled headers
target_precompile_headers(sglf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp")

# Compilation options
target_compile_options(sglf PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_compile_definitions(sglf PRIVATE WIN32_LEAN_AND_MEAN)

# Link dependencies
target_link_libraries(sglf
	PUBLIC
		vorbisfile
		vorbis
		ogg
		plutosvg
		plutovg
		gdiplus
		dsound
		windowscodecs
		opengl32
		user32
		gdi32
		ole32
)
