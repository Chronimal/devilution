
cmake_minimum_required (VERSION 3.10)

set(INTERFACE_HEADER_FILES
    include/smacker/smacker.h
)

set(INTERNAL_HEADER_FILES
    src/smk_bitstream.h
    src/smk_hufftree.h
    src/smk_malloc.h
)

set(SOURCE_FILES
    src/smacker.c
    src/smk_bitstream.c
    src/smk_hufftree.c
)

source_group("Interface Header Files" FILES ${INTERFACE_HEADER_FILES})
source_group("Internal Header Files" FILES ${INTERNAL_HEADER_FILES})

add_library(libsmacker STATIC
    ${INTERFACE_HEADER_FILES}
    ${INTERNAL_HEADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(libsmacker
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
