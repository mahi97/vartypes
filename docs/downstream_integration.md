# Downstream Integration Guide

## Overview

The modernized vartypes library provides CMake package config files for easy
integration via `find_package()`. Two library targets are available:

| Target | Description |
|---|---|
| `vartypes::vartypes-core` | Types, serialization (XML + JSON), factory, primitives |
| `vartypes::vartypes-widgets` | Qt tree view, model, delegate (requires `BUILD_GUI=ON`) |
| `vartypes::vartypes` | INTERFACE target linking both core and widgets |

## Method 1: find_package (Installed)

After installing vartypes:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=ON
cmake --build build --parallel
cmake --install build --prefix /usr/local
```

In your downstream CMakeLists.txt:

```cmake
find_package(vartypes REQUIRED)

# Full usage (core + GUI tree view)
target_link_libraries(my_app PRIVATE vartypes::vartypes)

# Core only (no tree view widget, but still requires Qt Widgets due to VarType base class)
target_link_libraries(my_app PRIVATE vartypes::vartypes-core)
```

## Method 2: add_subdirectory (Vendored)

Clone or add vartypes as a subdirectory/submodule:

```cmake
add_subdirectory(third_party/vartypes)

# Use the non-namespaced targets directly
target_link_libraries(my_app PRIVATE vartypes)
# or
target_link_libraries(my_app PRIVATE vartypes-core)
```

## Method 3: FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(vartypes
  GIT_REPOSITORY https://github.com/mahi97/vartypes.git
  GIT_TAG        copilot/modernize-qt-cpp-project  # or a release tag
)
FetchContent_MakeAvailable(vartypes)

target_link_libraries(my_app PRIVATE vartypes)
```

## Method 4: ExternalProject

For projects that prefer a separate build:

```cmake
include(ExternalProject)
ExternalProject_Add(vartypes_external
  GIT_REPOSITORY https://github.com/mahi97/vartypes.git
  GIT_TAG        copilot/modernize-qt-cpp-project
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DBUILD_GUI=ON
    -DBUILD_TESTS=OFF
)
```

## Important Notes

### Qt Dependency

`vartypes-core` depends on `Qt::Core`, `Qt::Gui`, and `Qt::Widgets` because
the `VarType` base class has virtual GUI methods (`createEditor`, `paint`,
etc.). This means even core-only usage requires a full Qt Widgets installation.
A future version may decouple this via an interface pattern.

### C++ Standard

vartypes requires C++14 or later. It uses `std::make_unique` and
`std::shared_ptr` extensively.

### Version Compatibility

The package config uses `SameMajorVersion` compatibility. Version 0.8.x
consumers should pin to `find_package(vartypes 0.8)`.

### Migrating from the Old Library Name

The old vartypes produced a single `libvartypes.so` / `vartypes.dll`. The
modernized version produces `libvartypes-core.so` and `libvartypes-widgets.so`.

If your project linked `-lvartypes` directly in linker flags, switch to
CMake's `target_link_libraries` with the targets above. The `vartypes`
INTERFACE target preserves backward compatibility for CMake-based consumers.
