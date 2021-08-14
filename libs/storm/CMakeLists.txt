
cmake_minimum_required (VERSION 3.10)
unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

set(INTERFACE_HEADER_FILES
    include/storm/storm.h
)

set(SOURCE_FILES
    src/storm.cpp
)

set(DEFINITION_FILES
    src/storm.def
)

source_group("Definition Files" FILES ${DEFINITION_FILES})
source_group("Interface Header Files" FILES ${INTERFACE_HEADER_FILES})

add_library(storm SHARED
    ${INTERFACE_HEADER_FILES}
    ${SOURCE_FILES}
    ${DEFINITION_FILES}
)

target_include_directories(storm
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(storm PRIVATE cxx_std_17)
