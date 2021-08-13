# CMakeList.txt : 3rdparty top-level CMake project file, do 3rdparty global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Include sub-projects.
add_subdirectory ("libpk")
add_subdirectory ("libstorm")
add_subdirectory ("libsmacker-1.1.1")
