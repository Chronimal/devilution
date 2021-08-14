# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)
#set(VS_STARTUP_PROJECT Diablo)
set(CMAKE_GENERATOR_PLATFORM Win32)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Diablo)

project ("Devilution")

# Include sub-projects.
add_subdirectory ("Diablo")
add_subdirectory ("libs")
add_subdirectory ("shared")

set_target_properties (DiabloUI PROPERTIES FOLDER Libraries)
set_target_properties (storm PROPERTIES FOLDER Libraries)
set_target_properties (ddshim PROPERTIES FOLDER Libraries)
set_target_properties (libpk PROPERTIES FOLDER Libraries)
set_target_properties (libsmacker PROPERTIES FOLDER Libraries)
