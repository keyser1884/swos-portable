# SWOS Port - Self-Contained Build

A self-contained, zero-dependency build of Sensible World of Soccer 96/97 for Windows.

## Credits

This project is a fork of the excellent [swos-port](https://github.com/zlatkok/swos-port) by **zlatkok**. The original project is an incredible achievement - a complete reverse-engineering and modern port of the classic DOS game using SDL.

All credit for the core game code, reverse engineering work, and game logic belongs to the original author. This fork focuses solely on simplifying the build process.

## What This Fork Does Differently

| Aspect | Original Project | This Fork |
|--------|-----------------|-----------|
| **Build System** | Meson | CMake |
| **Compiler** | Visual Studio 2015/2019 + MASM | MinGW GCC (bundled) |
| **Dependencies** | External SDL2 DLLs required | All libraries statically linked |
| **Tools Required** | VS, Python, gperf, Cygwin (optional) | None - everything bundled |
| **Build Process** | Multi-step with dependency chain | Single `build.bat` |
| **Output** | Executable + DLLs | Single standalone .exe |

## Why This Fork Exists

The original swos-port requires significant setup:
- Visual Studio 2015 or 2019
- MASM assembler
- Python 3.8+ with specific packages
- SDL2 development libraries
- Multiple configuration steps

This fork provides a **truly portable build** that:
- Bundles MinGW compiler in `tools/mingw32/`
- Bundles CMake in `tools/cmake/`
- Includes all SDL2 libraries pre-compiled for MinGW
- Produces a single executable with no DLL dependencies
- Builds with one double-click on `build.bat`

## Building

### Requirements

- Windows (tested on Windows 10/11)
- That's it.

### Build Steps

1. Clone or download this repository
2. Double-click `build.bat`
3. Find your executable in `bin/swos-port-Win32-Release.exe`

The build script automatically:
- Uses the bundled MinGW compiler
- Uses the bundled CMake
- Configures and builds the project
- Outputs a statically-linked executable

## Project Structure

```
swos-c-port/
├── 3rd-party/
│   ├── mingw32/      # Pre-compiled MinGW libraries (SDL2, SDL2_image, SDL2_mixer, minizip)
│   ├── SimpleIni/    # Header-only INI parser
│   └── dirent/       # POSIX directory functions for Windows
├── src/              # Game source code
├── tmp/              # Generated code (swos-cpp-gen output)
├── tools/
│   ├── cmake/        # Bundled CMake 3.28
│   └── mingw32/      # Bundled MinGW GCC
├── build.bat         # One-click build script
└── CMakeLists.txt    # CMake configuration
```

## Technical Changes

### GCC Compatibility Fixes

The original code used MSVC-specific behavior where `goto` can jump over variable initializations. GCC enforces standard C++ rules, so several functions in `playerFunctions.cpp` were modified to wrap variable declarations in explicit scopes.

### Static Linking

All dependencies are statically linked:
- SDL2
- SDL2_image
- SDL2_mixer
- minizip-ng

The resulting executable has no external DLL dependencies beyond standard Windows system libraries.

## Running the Game

You'll need the original SWOS game data files. Place them according to the original project's documentation.

## License

This fork maintains the same license as the original project. See the original repository for license details.

## Links

- **Original Project**: https://github.com/zlatkok/swos-port
- **Original Author**: zlatkok
- **Discord**: discord.gg/b275aMwjuX (original project community)
