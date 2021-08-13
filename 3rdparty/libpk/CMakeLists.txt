
cmake_minimum_required (VERSION 3.10)

set(INTERFACE_HEADER_FILES
    include/pk/pk.h
)

set(SOURCE_FILES
    src/crc32.c
    src/explode.c
    src/implode.c
)

add_library(libpk STATIC
    ${INTERFACE_HEADER_FILES}
    ${SOURCE_FILES}
)

source_group("Interface Header Files" FILES ${INTERFACE_HEADER_FILES})

target_include_directories(libpk
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

