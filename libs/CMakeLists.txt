# CMakeList.txt : libs top-level CMake project file, do libs global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Include sub-projects.
add_subdirectory ("ddshim")
add_subdirectory ("DiabloUI")
