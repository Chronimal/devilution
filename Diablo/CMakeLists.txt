
cmake_minimum_required (VERSION 3.10)

set(HEADER_FILES
    src/all.h
    src/appfat.h
    src/automap.h
    src/capture.h
    src/codec.h
    src/control.h
    src/cursor.h
    src/dead.h
    src/debug.h
    src/diablo.h
    src/doom.h
    src/drlg_l1.h
    src/drlg_l2.h
    src/drlg_l3.h
    src/drlg_l4.h
    src/dthread.h
    src/dx.h
    src/effects.h
    src/encrypt.h
    src/engine.h
    src/error.h
    src/fault.h
    src/gamemenu.h
    src/gendung.h
    src/gmenu.h
    src/help.h
    src/init.h
    src/interfac.h
    src/inv.h
    src/itemdat.h
    src/items.h
    src/lighting.h
    src/list.h
    src/loadsave.h
    src/logging.h
    src/mainmenu.h
    src/minitext.h
    src/misdat.h
    src/missiles.h
    src/monstdat.h
    src/monster.h
    src/movie.h
    src/mpqapi.h
    src/msg.h
    src/msgcmd.h
    src/multi.h
    src/nthread.h
    src/objdat.h
    src/objects.h
    src/pack.h
    src/palette.h
    src/path.h
    src/pfile.h
    src/player.h
    src/plrmsg.h
    src/portal.h
    src/quests.h
    src/render.h
    src/restrict.h
    src/scrollrt.h
    src/setmaps.h
    src/sha.h
    src/sound.h
    src/spelldat.h
    src/spells.h
    src/stores.h
    src/sync.h
    src/textdat.h
    src/themes.h
    src/tmsg.h
    src/town.h
    src/towners.h
    src/track.h
    src/trigs.h
    src/types.h
    src/wave.h
)

set(SOURCE_FILES
    src/appfat.cpp
    src/automap.cpp
    src/capture.cpp
    src/codec.cpp
    src/control.cpp
    src/cursor.cpp
    src/dead.cpp
    src/debug.cpp
    src/diablo.cpp
    src/doom.cpp
    src/drlg_l1.cpp
    src/drlg_l2.cpp
    src/drlg_l3.cpp
    src/drlg_l4.cpp
    src/dthread.cpp
    src/dx.cpp
    src/effects.cpp
    src/encrypt.cpp
    src/engine.cpp
    src/error.cpp
    src/fault.cpp
    src/gamemenu.cpp
    src/gendung.cpp
    src/gmenu.cpp
    src/help.cpp
    src/init.cpp
    src/interfac.cpp
    src/inv.cpp
    src/itemdat.cpp
    src/items.cpp
    src/lighting.cpp
    src/loadsave.cpp
    src/logging.cpp
    src/mainmenu.cpp
    src/minitext.cpp
    src/misdat.cpp
    src/missiles.cpp
    src/monstdat.cpp
    src/monster.cpp
    src/movie.cpp
    src/mpqapi.cpp
    src/msg.cpp
    src/msgcmd.cpp
    src/multi.cpp
    src/nthread.cpp
    src/objdat.cpp
    src/objects.cpp
    src/pack.cpp
    src/palette.cpp
    src/path.cpp
    src/pfile.cpp
    src/player.cpp
    src/plrmsg.cpp
    src/portal.cpp
    src/quests.cpp
    src/render.cpp
    src/restrict.cpp
    src/scrollrt.cpp
    src/setmaps.cpp
    src/sha.cpp
    src/sound.cpp
    src/spelldat.cpp
    src/spells.cpp
    src/stores.cpp
    src/sync.cpp
    src/textdat.cpp
    src/themes.cpp
    src/tmsg.cpp
    src/town.cpp
    src/towners.cpp
    src/track.cpp
    src/trigs.cpp
    src/wave.cpp
)

set(RESOURCE_FILES
    src/res/Diablo.ico
    src/res/Diablo.rc
    src/res/resource.h
)

source_group("Resource Files" FILES ${RESOURCE_FILES})

add_executable(Diablo WIN32
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${RESOURCE_FILES}
)

target_include_directories(Diablo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Diablo PRIVATE DiabloUI libstorm ddshim libpk shared)

target_compile_features(Diablo PRIVATE cxx_std_20)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (CMAKE_HOST_WIN32 AND NOT CMAKE_CROSSCOMPILING)
    GET_FILENAME_COMPONENT(DIABLO_PATH "[HKEY_CURRENT_USER\\SOFTWARE\\Blizzard Entertainment\\Diablo\\DelOpt0;Path0]" ABSOLUTE)
    if (EXISTS "${DIABLO_PATH}" AND IS_DIRECTORY "${DIABLO_PATH}")
        file(TO_NATIVE_PATH ${DIABLO_PATH} DIABLO_PATH)
        set_target_properties(Diablo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${DIABLO_PATH}")
    endif()
endif()
