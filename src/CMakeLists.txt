# SWOS Port Source Configuration
# This file lists all source files and configures the build

# Source files organized by module
set(SOURCES_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/crash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stdinc.cpp
)

set(SOURCES_AUDIO
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/audio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/audioOptionsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/chants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/comments.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/music.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/SampleTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/SoundSample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/sfx.cpp
)

set(SOURCES_CONTROLS
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/controlOptionsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/controls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/gameControlEvents.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/gameControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/quickConfigMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/selectGameControlEventsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/selectMatchControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/testControlsMenu.cpp
)

set(SOURCES_CONTROLS_JOYPADS
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/Joypad.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/JoypadConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/JoypadConfigRegister.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/JoypadElementValue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/JoypadGuidConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/joypads.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/SdlMappingParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/VirtualJoypad.cpp
)

set(SOURCES_CONTROLS_JOYPADS_UI
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/configureAxisMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/configureHatMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/configureTrackballMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/joypadConfigMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/joypadQuickConfigMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui/testJoypadMenu.cpp
)

set(SOURCES_CONTROLS_KEYBOARD
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/keyBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/KeyConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/redefineKeysMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/scancodes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/setupKeyboardMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard/testKeyboardMenu.cpp
)

set(SOURCES_CONTROLS_MOUSE
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/mouse/mouse.cpp
)

set(SOURCES_DATA
    ${CMAKE_CURRENT_SOURCE_DIR}/data/CompetitionData.cpp
)

set(SOURCES_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/debug/dump.cpp
)

set(SOURCES_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/files/embeddedAssets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/files/file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/files/selectFilesMenu.cpp
)

set(SOURCES_GAME
    ${CMAKE_CURRENT_SOURCE_DIR}/game/amigaMode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/color.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/darkRectangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/gameLoop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/gameTime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/playerNameDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/referee.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/replayExitMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/result.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/spinningLogo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/stats.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/updateGoals.cpp
)

set(SOURCES_GAME_AI
    ${CMAKE_CURRENT_SOURCE_DIR}/game/ai/ai.cpp
)

set(SOURCES_GAME_BALL
    ${CMAKE_CURRENT_SOURCE_DIR}/game/ball/ball.cpp
)

set(SOURCES_GAME_BENCH
    ${CMAKE_CURRENT_SOURCE_DIR}/game/bench/bench.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/bench/drawBench.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/bench/updateBench.cpp
)

set(SOURCES_GAME_PITCH
    ${CMAKE_CURRENT_SOURCE_DIR}/game/pitch/pitch.cpp
)

set(SOURCES_GAME_UPDATEPLAYERS
    ${CMAKE_CURRENT_SOURCE_DIR}/game/updatePlayers/playerFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/updatePlayers/updatePlayers.cpp
)

set(SOURCES_MENUS
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/careerMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/continueAbortMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/continueMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/mainMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/stadiumMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/versusMenu.cpp
)

set(SOURCES_MENUS_ENGINE
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/drawMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/MenuEntry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuAlloc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuBackground.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuItemRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuMouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menuProc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/menus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine/unpackMenu.cpp
)

set(SOURCES_OPTIONS
    ${CMAKE_CURRENT_SOURCE_DIR}/options/aboutMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options/options.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options/optionsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options/portedFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/options/portedFunctionsMenu.cpp
)

set(SOURCES_REPLAYS
    ${CMAKE_CURRENT_SOURCE_DIR}/replays/LegacyReplayConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/replays/ReplayDataStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/replays/replays.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/replays/replaysMenu.cpp
)

set(SOURCES_SPRITES
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/colorizeSprites.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/gameSprites.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/renderSprites.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/sprites.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/updateSprite.cpp
)

set(SOURCES_SPRITES_UTIL
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/util/loadTexture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/util/SharedTexture.cpp
)

# SWOS.cpp removed - SWOS::rand() is already defined in util/random.cpp
set(SOURCES_SWOS
)

set(SOURCES_TEXT
    ${CMAKE_CURRENT_SOURCE_DIR}/text/text.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/text/textInput.cpp
)

set(SOURCES_UTIL
    ${CMAKE_CURRENT_SOURCE_DIR}/util/hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/random.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/zip.cpp
)

set(SOURCES_VIDEO
    ${CMAKE_CURRENT_SOURCE_DIR}/video/assetManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/drawPrimitives.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/gameFieldMapping.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/overlay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/render.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/videoOptionsMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/windowManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/video/windowModeMenu.cpp
)

# Generated source files (from swos-cpp-gen tool)
set(SOURCES_GENERATED
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-01.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-02.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-03.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-04.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-05.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-06.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-07.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-08.cpp
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/vm.cpp
)

# Combine all sources
set(ALL_SOURCES
    ${SOURCES_ROOT}
    ${SOURCES_AUDIO}
    ${SOURCES_CONTROLS}
    ${SOURCES_CONTROLS_JOYPADS}
    ${SOURCES_CONTROLS_JOYPADS_UI}
    ${SOURCES_CONTROLS_KEYBOARD}
    ${SOURCES_CONTROLS_MOUSE}
    ${SOURCES_DATA}
    ${SOURCES_DEBUG}
    ${SOURCES_FILES}
    ${SOURCES_GAME}
    ${SOURCES_GAME_AI}
    ${SOURCES_GAME_BALL}
    ${SOURCES_GAME_BENCH}
    ${SOURCES_GAME_PITCH}
    ${SOURCES_GAME_UPDATEPLAYERS}
    ${SOURCES_MENUS}
    ${SOURCES_MENUS_ENGINE}
    ${SOURCES_OPTIONS}
    ${SOURCES_REPLAYS}
    ${SOURCES_SPRITES}
    ${SOURCES_SPRITES_UTIL}
    ${SOURCES_SWOS}
    ${SOURCES_TEXT}
    ${SOURCES_UTIL}
    ${SOURCES_VIDEO}
    ${SOURCES_GENERATED}
)

# Create the executable
add_executable(swos-port WIN32 ${ALL_SOURCES})

# Include directories
target_include_directories(swos-port PRIVATE
    # Source directories
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/controls
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/joypads/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/keyboard
    ${CMAKE_CURRENT_SOURCE_DIR}/controls/mouse
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/debug
    ${CMAKE_CURRENT_SOURCE_DIR}/files
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/game/ai
    ${CMAKE_CURRENT_SOURCE_DIR}/game/ball
    ${CMAKE_CURRENT_SOURCE_DIR}/game/bench
    ${CMAKE_CURRENT_SOURCE_DIR}/game/pitch
    ${CMAKE_CURRENT_SOURCE_DIR}/game/updatePlayers
    ${CMAKE_CURRENT_SOURCE_DIR}/menus
    ${CMAKE_CURRENT_SOURCE_DIR}/menus/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/options
    ${CMAKE_CURRENT_SOURCE_DIR}/replays
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites
    ${CMAKE_CURRENT_SOURCE_DIR}/sprites/util
    ${CMAKE_CURRENT_SOURCE_DIR}/swos
    ${CMAKE_CURRENT_SOURCE_DIR}/text
    ${CMAKE_CURRENT_SOURCE_DIR}/util
    ${CMAKE_CURRENT_SOURCE_DIR}/video
    # Generated headers
    ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}
    ${CMAKE_SOURCE_DIR}/tmp/mnu2h
    ${CMAKE_SOURCE_DIR}/tmp/assets
    # 3rd-party includes (MinGW)
    ${CMAKE_SOURCE_DIR}/3rd-party/mingw32/include
    ${CMAKE_SOURCE_DIR}/3rd-party/mingw32/include/SDL2
    ${CMAKE_SOURCE_DIR}/3rd-party/SimpleIni
    ${CMAKE_SOURCE_DIR}/3rd-party/dirent
)

# Preprocessor definitions
target_compile_definitions(swos-port PRIVATE
    DISABLE_ALIGNMENT_CHECKS=1
    SWOS_VM
    NOMINMAX
    _CRT_SECURE_NO_WARNINGS
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<BOOL:${SWOS_EMBED_ASSETS}>:EMBED_ASSETS>
)

# Compiler options
if(MSVC)
    target_compile_options(swos-port PRIVATE
        /W3                          # Warning level 3
        /wd4731                      # Disable warning 4731 (frame pointer modified)
        /MP                          # Multiprocessor compilation
        $<$<CONFIG:Release>:/O2>     # Max speed optimization
        $<$<CONFIG:Release>:/GL>     # Whole program optimization
        $<$<CONFIG:Release>:/Oi>     # Enable intrinsic functions
        $<$<CONFIG:Release>:/Gy>     # Function-level linking
        $<$<CONFIG:Release>:/GS->    # Disable buffer security check for perf
    )

    # Disable all warnings for generated swos-*.cpp files
    set_source_files_properties(
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-01.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-02.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-03.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-04.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-05.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-06.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-07.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-08.cpp
        PROPERTIES COMPILE_FLAGS "/w"
    )

    # Linker options for Release
    target_link_options(swos-port PRIVATE
        $<$<CONFIG:Release>:/LTCG>           # Link-time code generation
        $<$<CONFIG:Release>:/OPT:REF>        # Optimize references
        $<$<CONFIG:Release>:/OPT:ICF>        # COMDAT folding
    )

    # Disable safe exception handlers for x86
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_options(swos-port PRIVATE /SAFESEH:NO)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(swos-port PRIVATE
        -Wall
        -Wno-unused-variable
        -Wno-unused-but-set-variable
        -Wno-narrowing
        -Wno-parentheses
        -fpermissive
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:Release>:-fomit-frame-pointer>
    )

    # Disable all warnings for generated swos-*.cpp files
    set_source_files_properties(
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-01.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-02.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-03.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-04.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-05.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-06.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-07.cpp
        ${CMAKE_SOURCE_DIR}/tmp/swos-cpp-gen-${PLATFORM_ARCH}/swos-08.cpp
        PROPERTIES COMPILE_FLAGS "-w"
    )

    # Static linking for MinGW
    target_link_options(swos-port PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# Precompiled headers
target_precompile_headers(swos-port PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/stdinc.h>"
)

# Platform-specific lib directory suffix
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_PLATFORM "win64")
else()
    set(LIB_PLATFORM "win32")
endif()

target_compile_definitions(swos-port PRIVATE SDL_MAIN_HANDLED)

# Library linking - different paths for MinGW vs MSVC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # MinGW libraries
    set(MINGW_LIB_DIR "${CMAKE_SOURCE_DIR}/3rd-party/mingw32")

    target_include_directories(swos-port PRIVATE
        ${MINGW_LIB_DIR}/include
        ${MINGW_LIB_DIR}/include/SDL2
    )

    target_link_directories(swos-port PRIVATE
        ${MINGW_LIB_DIR}/lib
    )

    target_link_libraries(swos-port PRIVATE
        # SDL2 static (MinGW naming: libSDL2.a)
        SDL2
        SDL2main
        # SDL2_image static
        SDL2_image
        # SDL2_mixer static (includes audio codecs)
        SDL2_mixer
        # Minizip
        minizip
        # Windows crypto for minizip
        bcrypt
        # Windows system libraries
        winmm
        imm32
        version
        setupapi
        cfgmgr32
        ole32
        oleaut32
        gdi32
        user32
    )
else()
    # MSVC libraries (from 3rd-party/static/)
    set(STATIC_LIB_DIR "${CMAKE_SOURCE_DIR}/3rd-party/static/${LIB_PLATFORM}")

    target_include_directories(swos-port PRIVATE
        ${CMAKE_SOURCE_DIR}/3rd-party/static/include
    )

    target_link_directories(swos-port PRIVATE
        ${STATIC_LIB_DIR}/lib
    )

    target_link_libraries(swos-port PRIVATE
        # SDL2 static
        SDL2-static
        SDL2main
        # SDL2_image static
        SDL2_image-static
        # SDL2_mixer static
        SDL2_mixer-static
        # Compression libraries
        minizip
        zlib
        bz2
        lzma
        zstd
        # Image format libraries
        libpng16
        # Audio codec libraries
        modplug
        ogg
        vorbis
        vorbisfile
        libwavpack
        libxmp-static
        # Windows system libraries
        winmm
        imm32
        version
        setupapi
        cfgmgr32
        bcrypt
    )
endif()

# Use static runtime
set_property(TARGET swos-port PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")


# Output configuration
set_target_properties(swos-port PROPERTIES
    OUTPUT_NAME "swos-port-$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,Win32>-$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    # Ensure consistent output directories across configurations
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin"
)

# Generate map file for Release builds
if(MSVC)
    set_target_properties(swos-port PROPERTIES
        LINK_FLAGS_RELEASE "/MAP"
    )
endif()

# DPI awareness manifest (Windows)
if(WIN32)
    set_target_properties(swos-port PROPERTIES
        VS_DPI_AWARE "PerMonitor"
    )
endif()

# Source grouping for Visual Studio IDE
source_group("Source Files" FILES ${SOURCES_ROOT})
source_group("Source Files\\audio" FILES ${SOURCES_AUDIO})
source_group("Source Files\\controls" FILES ${SOURCES_CONTROLS})
source_group("Source Files\\controls\\joypads" FILES ${SOURCES_CONTROLS_JOYPADS})
source_group("Source Files\\controls\\joypads\\ui" FILES ${SOURCES_CONTROLS_JOYPADS_UI})
source_group("Source Files\\controls\\keyboard" FILES ${SOURCES_CONTROLS_KEYBOARD})
source_group("Source Files\\controls\\mouse" FILES ${SOURCES_CONTROLS_MOUSE})
source_group("Source Files\\data" FILES ${SOURCES_DATA})
source_group("Source Files\\debug" FILES ${SOURCES_DEBUG})
source_group("Source Files\\files" FILES ${SOURCES_FILES})
source_group("Source Files\\game" FILES ${SOURCES_GAME})
source_group("Source Files\\game\\ai" FILES ${SOURCES_GAME_AI})
source_group("Source Files\\game\\ball" FILES ${SOURCES_GAME_BALL})
source_group("Source Files\\game\\bench" FILES ${SOURCES_GAME_BENCH})
source_group("Source Files\\game\\pitch" FILES ${SOURCES_GAME_PITCH})
source_group("Source Files\\game\\updatePlayers" FILES ${SOURCES_GAME_UPDATEPLAYERS})
source_group("Source Files\\menus" FILES ${SOURCES_MENUS})
source_group("Source Files\\menus\\engine" FILES ${SOURCES_MENUS_ENGINE})
source_group("Source Files\\options" FILES ${SOURCES_OPTIONS})
source_group("Source Files\\replays" FILES ${SOURCES_REPLAYS})
source_group("Source Files\\sprites" FILES ${SOURCES_SPRITES})
source_group("Source Files\\sprites\\util" FILES ${SOURCES_SPRITES_UTIL})
source_group("Source Files\\swos" FILES ${SOURCES_SWOS})
source_group("Source Files\\text" FILES ${SOURCES_TEXT})
source_group("Source Files\\util" FILES ${SOURCES_UTIL})
source_group("Source Files\\video" FILES ${SOURCES_VIDEO})
source_group("Generated Files" FILES ${SOURCES_GENERATED})
