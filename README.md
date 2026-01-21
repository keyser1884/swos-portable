# SWOS Portable

A pure C++ port of Sensible World of Soccer 96/97 with a self-contained, zero-dependency build system.

## What This Project Is

This is a **complete C++ port** of SWOS that runs the game through a virtual machine emulating the original CPU architecture. Unlike the original codebase which relies heavily on assembly, this version is 100% C++ and can be compiled with any standard C++ compiler.

The project includes:
- **SwosVM**: A virtual machine that emulates 68000 (A0-A6, D0-D7) and x86 registers
- **~520,000 lines of generated C++** that implement the original game logic
- **Self-contained build**: Bundled MinGW compiler and CMake - no external tools required
- **Static linking**: Single executable with no DLL dependencies

## Comparison with Original Project

This is a fork of [swos-port](https://github.com/zlatkok/swos-port) by **zlatkok**, which provided the foundational reverse-engineering work and SDL integration.

| Aspect | Original (zlatkok/swos-port) | This Fork |
|--------|------------------------------|-----------|
| **Code** | 59.7% Assembly, 31.8% C++ | 100% C++ |
| **Approach** | Hybrid assembly/C++ | Pure C++ virtual machine |
| **Compiler** | Visual Studio + MASM | Any C++ compiler (GCC, Clang, MSVC) |
| **Build System** | Meson | CMake |
| **Dependencies** | External SDL2 DLLs | All statically linked |
| **Tools Required** | VS, Python, MASM, etc. | None - everything bundled |
| **Output** | Executable + DLLs | Single standalone .exe |

## Building

### Requirements

- Windows (tested on Windows 10/11)
- That's it.

### Build Steps

1. Clone or download this repository
2. Double-click `build.bat`
3. Find your executable in `bin/swos-port-Win32-Release.exe`

The build script uses the bundled MinGW compiler and CMake. No external tools or configuration needed.

## Project Structure

```
swos-portable/
├── 3rd-party/
│   ├── mingw32/      # Pre-compiled MinGW libraries (SDL2, SDL2_image, SDL2_mixer, minizip)
│   ├── SimpleIni/    # Header-only INI parser
│   └── dirent/       # POSIX directory functions for Windows
├── src/              # Game source code (~280k lines)
├── tmp/
│   └── swos-cpp-gen-32/  # Generated VM code (~240k lines)
├── tools/
│   ├── cmake/        # Bundled CMake 3.28
│   └── mingw32/      # Bundled MinGW GCC
├── build.bat         # One-click build script
└── CMakeLists.txt    # CMake configuration
```

## Technical Details

### The Virtual Machine

The core of this port is `SwosVM`, which provides:
- Emulated 68000 registers (A0-A6, D0-D7) from the original Amiga version
- Emulated x86 registers (eax, ebx, ecx, edx, esi, edi, ebp)
- Memory simulation matching the original game's memory layout
- CPU flags emulation for condition codes

The original game logic has been translated to C++ functions that manipulate these virtual registers and memory, allowing the game to run without any actual assembly code.

### GCC Compatibility

Several fixes were made to compile with GCC/MinGW:
- Standard C++ compliance for `goto` statements crossing variable initializations
- Wrapped variable declarations in explicit scopes where needed

## Running the Game

You'll need the original SWOS game data files. Place them according to the original project's documentation.

## Credits

- **zlatkok** - Original [swos-port](https://github.com/zlatkok/swos-port) project, reverse engineering, SDL integration
- **Sensible Software** - Original Sensible World of Soccer

## Links

- **Original Project**: https://github.com/zlatkok/swos-port
- **Discord**: discord.gg/b275aMwjuX (original project community)
