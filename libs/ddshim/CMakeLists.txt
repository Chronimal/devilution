
cmake_minimum_required (VERSION 3.10)

# --- Source ---

set(HEADER_FILES
    src/d3dkmt.hpp
    src/ddshim.hpp
    src/deviceresources.hpp
    src/directdraw.hpp
    src/directdrawpalette.hpp
    src/directdrawsurface.hpp
)

set(PSHADER_FILE
    src/shaders/pixelshader.hlsl
)

set(VSHADER_FILE
    src/shaders/vertexshader.hlsl
)

set(SOURCE_FILES
    src/ddshim.cpp
    src/deviceresources.cpp
    src/directdraw.cpp
    src/directdrawpalette.cpp
    src/directdrawsurface.cpp
)

# --- VS Solution Explorer Layout ---

set(SHADER_FILES
    ${PSHADER_FILE}
    ${VSHADER_FILE}
)

source_group("Shader Files" FILES ${SHADER_FILES})

# --- Build ---

set_source_files_properties(
    ${PSHADER_FILE}
    PROPERTIES
        VS_SHADER_VARIABLE_NAME g_pixelShader
        VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h"
        VS_SHADER_TYPE Pixel
        VS_SHADER_MODEL 4.0
        VS_SHADER_ENTRYPOINT main
)

set_source_files_properties(
    ${VSHADER_FILE}
    PROPERTIES
        VS_SHADER_VARIABLE_NAME g_vertexShader
        VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h"
        VS_SHADER_TYPE Vertex
        VS_SHADER_MODEL 4.0_level_9_3
        VS_SHADER_ENTRYPOINT main
)

add_library(ddshim STATIC
    ${HEADER_FILES}
    ${SHADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(ddshim
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
)

target_compile_features(ddshim PRIVATE cxx_std_20)
