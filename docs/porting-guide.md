# Porting to Linux, macOS, and Web Browsers

This document analyzes what's required to build SWOS-C-Port on Linux, macOS, and web browsers (via WebAssembly).

## Overview

The codebase is well-designed for cross-platform support thanks to SDL2. Most game logic and rendering code requires no changes - the main work is CMake configuration.

## Current Platform Support

- **Windows**: Fully supported (MinGW and MSVC)
- **Android**: Partial support exists (JNI guards present)
- **Linux/macOS**: Not yet configured, but code is portable
- **WebAssembly**: Not yet configured, but SDL2 supports Emscripten

## Required Changes

### 1. CMakeLists.txt - Executable Setup

Remove or guard the `WIN32` flag in `src/CMakeLists.txt` (line 241):

```cmake
if(WIN32)
    add_executable(swos-port WIN32 ${ALL_SOURCES})
else()
    add_executable(swos-port ${ALL_SOURCES})
endif()
```

### 2. CMakeLists.txt - Library Detection

Replace hardcoded Windows libraries with platform detection:

```cmake
if(WIN32)
    # Current MinGW/MSVC library linking code
    target_link_libraries(swos-port
        winmm imm32 version setupapi cfgmgr32
        ole32 oleaut32 gdi32 user32 bcrypt
        # ... bundled SDL2 libraries
    )
elseif(APPLE)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    target_link_libraries(swos-port
        ${SDL2_LIBRARIES}
        SDL2_image
        SDL2_mixer
    )
else()
    # Linux
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    target_link_libraries(swos-port
        ${SDL2_LIBRARIES}
        SDL2_image
        SDL2_mixer
    )
endif()
```

### 3. CMakeLists.txt - Preprocessor Definitions

Guard Windows-only definitions:

```cmake
target_compile_definitions(swos-port PRIVATE
    DISABLE_ALIGNMENT_CHECKS=1
    SWOS_VM
    SDL_MAIN_HANDLED
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<BOOL:${SWOS_EMBED_ASSETS}>:EMBED_ASSETS>
)

if(MSVC)
    target_compile_definitions(swos-port PRIVATE
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )
elseif(WIN32)
    target_compile_definitions(swos-port PRIVATE
        NOMINMAX
    )
endif()
```

### 4. Windows Manifest

Guard the DPI awareness manifest (lines 478-483):

```cmake
if(WIN32)
    # Windows DPI awareness manifest generation
endif()
```

## Platform Dependencies

### Linux (Debian/Ubuntu)

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev cmake build-essential
```

### Linux (Fedora/RHEL)

```bash
sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel cmake gcc-c++
```

### macOS (Homebrew)

```bash
brew install sdl2 sdl2_image sdl2_mixer cmake
```

## What Already Works (No Changes Needed)

| Component | Location | Notes |
|-----------|----------|-------|
| Graphics/Video | `src/video/` | Pure SDL2 rendering |
| Audio | `src/audio/` | SDL_mixer, fully portable |
| Input | `src/controls/` | SDL keyboard/mouse/joystick |
| File paths | `src/files/file.cpp` | Already has Linux path normalization |
| Crash handler | `src/crash.cpp` | Stubbed out (CrashRpt disabled) |
| Debug heap | `src/main.cpp` | Already guarded with `#ifdef _WIN32` |
| VM code | `tmp/swos-cpp-gen-*/` | Platform-independent |

## Platform-Specific Code Locations

### Windows-Only Code

- `src/files/file.cpp:10-39` - Uses `<direct.h>` for directory ops (guarded)
- `src/main.cpp:14-22` - Debug heap setup (guarded)
- `src/crash.cpp` - CrashRpt integration (disabled)

### Android-Specific Code

- `src/controls/keyboard/keyboard.cpp:19-28` - JNI keyboard detection
- `src/util/log.cpp:6-64` - Android logging
- `src/video/windowManager.cpp:18-22` - Fullscreen default
- `src/main.cpp:26-28` - SDL_main handling

## macOS-Specific Considerations

| Item | Required? | Notes |
|------|-----------|-------|
| App bundle (.app) | Optional | Use `MACOSX_BUNDLE` in CMake |
| Code signing | Optional | Only for distribution |
| Retina/HiDPI | Automatic | SDL2 handles this |
| Apple Silicon (ARM) | Should work | SDL2 supports M1/M2/M3 natively |

### Creating a macOS App Bundle

```cmake
if(APPLE)
    set_target_properties(swos-port PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/platform/macos/Info.plist
    )
endif()
```

## Third-Party Dependencies

| Library | Windows | Linux/macOS |
|---------|---------|-------------|
| SDL2 | Bundled in `3rd-party/mingw32/` | System package |
| SDL2_image | Bundled | System package |
| SDL2_mixer | Bundled | System package |
| SimpleIni | Header-only, cross-platform | Same |
| dirent | Provides POSIX API for Windows | Use system `<dirent.h>` |
| minizip | Bundled | System package or bundled |
| libpng | Bundled | System package (SDL2_image dep) |
| libogg/libvorbis | Bundled | System package (SDL2_mixer dep) |

## Build Scripts

### Linux/macOS Build Script (build.sh)

```bash
#!/bin/bash
set -e

BUILD_TYPE=${1:-Release}
BUILD_DIR="build-linux"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build . --parallel $(nproc)

echo "Build complete: $BUILD_DIR/swos-port"
```

## WebAssembly (Browser) Support

WebAssembly allows the game to run directly in web browsers without installation. SDL2 has mature Emscripten support, making this a viable target.

### Browser Compatibility

| Browser | WebAssembly | WebGL (SDL2) | Web Audio | Gamepad API |
|---------|-------------|--------------|-----------|-------------|
| Chrome | Yes | Yes | Yes | Yes |
| Firefox | Yes | Yes | Yes | Yes |
| Safari | Yes | Yes | Yes | Yes |
| Edge | Yes | Yes | Yes | Yes |
| Mobile Chrome | Yes | Yes | Yes | Limited |
| Mobile Safari | Yes | Yes | Yes | Limited |

**Coverage**: ~96% of all browsers worldwide.

### How It Works

- **Graphics**: SDL2 renders via WebGL (hardware accelerated)
- **Audio**: SDL_mixer uses Web Audio API
- **Input**: Keyboard, mouse, and gamepad via browser APIs
- **Fullscreen**: Supported (requires user interaction to trigger)

### Required Changes for WebAssembly

#### 1. Emscripten Toolchain Setup

```bash
# Install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

#### 2. CMake Configuration for Emscripten

```cmake
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # SDL2 is built into Emscripten
    set(USE_FLAGS "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_MIXER=2")
    set(USE_FLAGS "${USE_FLAGS} -sSDL2_IMAGE_FORMATS=['png']")
    set(USE_FLAGS "${USE_FLAGS} -sSDL2_MIXER_FORMATS=['ogg']")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")

    # Link flags
    set(LINK_FLAGS "-sWASM=1 -sALLOW_MEMORY_GROWTH=1")
    set(LINK_FLAGS "${LINK_FLAGS} -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']")

    # Embed or preload assets
    set(LINK_FLAGS "${LINK_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/assets@/assets")

    set_target_properties(swos-port PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
endif()
```

#### 3. Build Command

```bash
mkdir build-web && cd build-web
emcmake cmake ..
emmake make
# Produces: swos-port.html, swos-port.js, swos-port.wasm, swos-port.data
```

### Challenges and Solutions

| Challenge | Impact | Solution |
|-----------|--------|----------|
| Asset loading | Large initial download | Use `--preload-file` to bundle assets, show loading progress |
| File system | No disk access | Use Emscripten's virtual FS (MEMFS) |
| Save games | Can't write to disk | Use IndexedDB via Emscripten's IDBFS |
| Main loop | Browser requires async | Use `emscripten_set_main_loop()` |
| Memory | Default 16MB limit | Use `-sALLOW_MEMORY_GROWTH=1` |
| 32-bit pointers | VM uses `PTR32` macro | Test thoroughly; WASM is 32-bit by default |

### File System Adaptation

For save games and persistent storage:

```cpp
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void initFileSystem() {
    // Mount IndexedDB for persistent storage
    EM_ASM(
        FS.mkdir('/save');
        FS.mount(IDBFS, {}, '/save');
        FS.syncfs(true, function(err) {
            if (err) console.error('FS sync error:', err);
        });
    );
}

void saveToStorage() {
    // Sync memory FS to IndexedDB
    EM_ASM(
        FS.syncfs(false, function(err) {
            if (err) console.error('Save error:', err);
        });
    );
}
#endif
```

### Main Loop Adaptation

Browsers require yielding control back to the event loop:

```cpp
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void mainLoopIteration() {
    // Single frame of game logic
    processInput();
    updateGame();
    render();
}

int main() {
    initialize();
    emscripten_set_main_loop(mainLoopIteration, 0, 1);
    return 0;
}
#else
int main() {
    initialize();
    while (running) {
        mainLoopIteration();
    }
    return 0;
}
#endif
```

### HTML Shell Template

Create a custom HTML wrapper for branding and loading screen:

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>SWOS Port</title>
    <style>
        canvas { display: block; margin: 0 auto; }
        #loading { text-align: center; padding: 20px; }
    </style>
</head>
<body>
    <div id="loading">Loading... <span id="progress">0%</span></div>
    <canvas id="canvas"></canvas>
    <script>
        var Module = {
            canvas: document.getElementById('canvas'),
            setStatus: function(text) {
                document.getElementById('loading').innerText = text;
            },
            monitorRunDependencies: function(left) {
                if (left === 0) {
                    document.getElementById('loading').style.display = 'none';
                }
            }
        };
    </script>
    {{{ SCRIPT }}}
</body>
</html>
```

Use with: `--shell-file shell.html`

### Distribution Benefits

- **No installation**: Users just visit a URL
- **Cross-platform**: Works on any device with a modern browser
- **Easy updates**: Update server files, users get new version immediately
- **Shareable**: Send a link to anyone
- **Chrome OS**: Native browser support without Linux container

### WebAssembly Build Script (build-web.sh)

```bash
#!/bin/bash
set -e

# Ensure Emscripten is available
if ! command -v emcc &> /dev/null; then
    echo "Emscripten not found. Run: source /path/to/emsdk/emsdk_env.sh"
    exit 1
fi

BUILD_DIR="build-web"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make -j$(nproc)

echo "Build complete!"
echo "To test locally: cd $BUILD_DIR && python3 -m http.server 8080"
echo "Then open: http://localhost:8080/swos-port.html"
```

## Estimated Effort

### Native Platforms (Linux/macOS)

- **CMake changes**: 2-4 hours
- **Testing on Linux**: 1-2 hours
- **Testing on macOS**: 1-2 hours
- **App bundle setup (optional)**: 1-2 hours

### WebAssembly

- **CMake Emscripten configuration**: 2-3 hours
- **Main loop adaptation**: 1-2 hours
- **File system / save game handling**: 2-4 hours
- **Asset bundling and loading**: 1-2 hours
- **HTML shell and loading screen**: 1-2 hours
- **Testing and optimization**: 2-4 hours

**Total for WebAssembly**: 10-17 hours (medium effort)

The codebase is well-prepared for cross-platform support. SDL2 handles all the heavy lifting for graphics, audio, and input abstraction.
