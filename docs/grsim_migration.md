# grSim Migration Guide

This document describes how to migrate grSim from the old vartypes library
to the modernized version.

## Old Style (grSim current)

grSim currently uses `find_package(VarTypes)` with a fallback to
`ExternalProject`:

```cmake
find_package(VarTypes)

if(NOT VARTYPES_FOUND)
  include(ExternalProject)
  ExternalProject_Add(vartypes_external
    GIT_REPOSITORY    https://github.com/jpfeltracco/vartypes
    GIT_TAG           origin/jpfeltracco/build_static
    CMAKE_ARGS        "-DVARTYPES_BUILD_STATIC=ON;-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
    STEP_TARGETS install
  )
endif()

target_include_directories(${app} PRIVATE ${VARTYPES_INCLUDE_DIRS})
list(APPEND libs ${VARTYPES_LIBRARIES})
```

## Recommended New Style

### Option A: FetchContent (simplest)

```cmake
include(FetchContent)
FetchContent_Declare(vartypes
  GIT_REPOSITORY https://github.com/mahi97/vartypes.git
  GIT_TAG        osx-fix  # update to a release tag when available
)
set(BUILD_GUI ON CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(vartypes)

# Then in your target:
target_link_libraries(grSim PRIVATE vartypes)
```

### Option B: find_package (installed vartypes)

First, install the modernized vartypes:
```bash
cd vartypes
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=ON
cmake --build build --parallel
cmake --install build --prefix /usr/local
```

Then in grSim's CMakeLists.txt:
```cmake
find_package(vartypes REQUIRED)
target_link_libraries(grSim PRIVATE vartypes::vartypes)
```

Note: The package name changed from `VarTypes` (old) to `vartypes` (new).

### Option C: add_subdirectory (if vendored)

```cmake
add_subdirectory(libs/vartypes)
target_link_libraries(grSim PRIVATE vartypes)
```

## Migration Checklist

1. **Remove old find_package and ExternalProject code** for VarTypes
2. **Add FetchContent or find_package(vartypes)** as shown above
3. **Replace link lines**: `${VARTYPES_LIBRARIES}` -> `vartypes` or `vartypes::vartypes`
4. **Remove include_directories**: the vartypes target provides include paths transitively
5. **No header changes needed**: all existing `#include "VarTypes.h"`, `#include "VarList.h"` etc. continue to work
6. **No API changes**: `VarXML::read()`, `VarXML::write()`, `VarList::addChild()` etc. are unchanged
7. **Optional**: Add `#include "VarJSON.h"` to use the new JSON serialization

## What Changed

| Aspect | Old | New |
|---|---|---|
| Package name | `VarTypes` | `vartypes` |
| Library | single `libvartypes` | `libvartypes-core` + `libvartypes-widgets` |
| CMake target | `vartypes` (SHARED) | `vartypes` (INTERFACE -> core + widgets) |
| C++ standard | C++11 | C++14 |
| Serialization | XML only | XML + JSON |
| Qt version | Qt5 or Qt6 | Qt5 or Qt6 (unchanged) |

## Minimal Complete Example

```cmake
cmake_minimum_required(VERSION 3.16)
project(myapp)

include(FetchContent)
FetchContent_Declare(vartypes
  GIT_REPOSITORY https://github.com/mahi97/vartypes.git
  GIT_TAG        osx-fix
)
set(BUILD_GUI ON CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(vartypes)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE vartypes)
```
