cmake_minimum_required(VERSION 3.16)
project(swos-port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(SWOS_EMBED_ASSETS "Embed assets in executable" OFF)

# Platform architecture for generated files path (32 or 64)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_ARCH "64")
else()
    set(PLATFORM_ARCH "32")
endif()

# Use swos-port directory as the project root for paths
# The actual source files reference paths relative to the swos-port parent
set(SWOS_ROOT "${CMAKE_SOURCE_DIR}")

add_subdirectory(src)
