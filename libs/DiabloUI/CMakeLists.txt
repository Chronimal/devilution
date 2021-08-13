
cmake_minimum_required (VERSION 3.10)

set(INTERFACE_HEADER_FILES
    include/ui/diabloui.h
)

set(INTERNAL_HEADER_FILES
    src/inc.h
    src/_temp_funcs.h
)

set(SOURCE_FILES
    src/_temp_data.cpp
    src/artfont.cpp
    src/bnetgw.cpp
    src/bn_prof.cpp
    src/connect.cpp
    src/copyprot.cpp
    src/cr8game.cpp
    src/creadung.cpp
    src/creastat.cpp
    src/credits.cpp
    src/diabedit.cpp
    src/diabloui.cpp
    src/dirlink.cpp
    src/disclaim.cpp
    src/doom.cpp
    src/entdial.cpp
    src/entname.cpp
    src/fade.cpp
    src/focus.cpp
    src/local.cpp
    src/mainmenu.cpp
    src/modem.cpp
    src/modmstat.cpp
    src/okcancel.cpp
    src/progress.cpp
    src/sbar.cpp
    src/selclass.cpp
    src/selconn.cpp
    src/seldial.cpp
    src/selgame.cpp
    src/selhero.cpp
    src/selipx.cpp
    src/sellist.cpp
    src/selload.cpp
    src/selmodem.cpp
    src/selregn.cpp
    src/selyesno.cpp
    src/title.cpp
    src/titlesnd.cpp
)

set(RESOURCE_FILES
    src/res/diablocu.cur
    src/res/diabloib.cur
    src/res/diabloui.aps
    src/res/diabloui.rc
    src/res/diablo_a.cur
    src/res/diablo_l.cur
    src/res/resource.h
)

set(DEFINITION_FILES
    src/diabloui.def
)

source_group("Definition Files" FILES ${DEFINITION_FILES})
source_group("Interface Header Files" FILES ${INTERFACE_HEADER_FILES})
source_group("Internal Header Files" FILES ${INTERNAL_HEADER_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

add_library(DiabloUI SHARED
    ${INTERFACE_HEADER_FILES}
    ${INTERNAL_HEADER_FILES}
    ${SOURCE_FILES}
    ${RESOURCE_FILES}
    ${DEFINITION_FILES}
)

target_include_directories(DiabloUI
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DiabloUI PRIVATE libstorm shared)

target_compile_features(DiabloUI PRIVATE cxx_std_20)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

