cmake_minimum_required(VERSION 3.29)
project(sglf VERSION 0.6.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies
find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES_SGLF CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/sglf/*.cpp")

# Define the library
add_library(sglf ${SOURCES_SGLF})
add_library(sglf::sglf ALIAS sglf)

# Include directories
target_include_directories(sglf PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/sglf")

# Precompiled headers
target_precompile_headers(sglf PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/sglf/pch.hpp")

# Compilation options
target_compile_options(sglf PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_compile_definitions(sglf PRIVATE WIN32_LEAN_AND_MEAN)

# Link dependencies
target_link_libraries(sglf
	PUBLIC
		Ogg::ogg
		Vorbis::vorbis
		Vorbis::vorbisfile
		glm::glm-header-only
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

# Export targets
export(
	TARGETS sglf
	FILE "${CMAKE_CURRENT_BINARY_DIR}/sglfTargets.cmake"
	NAMESPACE sglf::
)

# Configure package config
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/sglfConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/sglfConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/sglfConfig.cmake"
	INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

# Test Executable
file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/test/*.cpp")
add_executable(test ${SOURCES_TEST})
target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/test" "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_precompile_headers(test REUSE_FROM sglf)
target_compile_options(test PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)
target_compile_definitions(test PRIVATE WIN32_LEAN_AND_MEAN)
target_link_libraries(test PRIVATE sglf)
