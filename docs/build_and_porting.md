# Build and Porting Guide

## Prerequisites

- **CMake** 3.16 or later
- **Qt** 5.15+ or Qt 6.5+ (Core, Gui, Widgets modules)
- **C++14** compatible compiler

## Building

```bash
# Basic build (core + widgets)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# With all options
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_GUI=ON \
  -DBUILD_EXAMPLES=ON \
  -DBUILD_TESTS=ON
cmake --build build --parallel

# Core-only build (no GUI widgets)
cmake -B build -DBUILD_GUI=OFF
cmake --build build --parallel
```

## Build Options

| Option | Default | Description |
|---|---|---|
| `BUILD_GUI` | ON | Build `vartypes-widgets` GUI library |
| `BUILD_EXAMPLES` | OFF | Build example applications (requires `BUILD_GUI=ON`) |
| `BUILD_TESTS` | OFF | Build and enable unit tests |

## Library Targets

| Target | Description |
|---|---|
| `vartypes-core` | Core library: types, serialization, factory, XML/JSON backends |
| `vartypes-widgets` | GUI library: tree view, item delegate, model (requires Qt Widgets) |
| `vartypes` | Backward-compatible INTERFACE target linking both core and widgets |

## Linking

### Modern CMake (recommended)
```cmake
find_package(vartypes REQUIRED)
target_link_libraries(my_app PRIVATE vartypes-core)  # Non-GUI usage
target_link_libraries(my_app PRIVATE vartypes)        # Full GUI usage
```

### Direct subdirectory
```cmake
add_subdirectory(vartypes)
target_link_libraries(my_app PRIVATE vartypes-core)
```

## Running Tests

```bash
cmake -B build -DBUILD_TESTS=ON
cmake --build build --parallel
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
```

## Qt Version Support

The build system auto-detects Qt6 first, then falls back to Qt5. Both are fully supported:
- **Qt 5.15.x**: Fully supported
- **Qt 6.5.x**: Fully supported

## Platform Support

| Platform | Qt Version | Status |
|---|---|---|
| Ubuntu 22.04 | Qt 5.15, Qt 6.5 | ✅ |
| Ubuntu 24.04 | Qt 5.15, Qt 6.5 | ✅ |
| macOS 14/15 | Qt 6.5 | ✅ |
| Windows | Qt 6.5 | ✅ |

## Porting from Old Build System

### Changes from legacy CMake
1. **C++ standard**: Upgraded from C++11 to C++14
2. **Global flags removed**: No more `CMAKE_CXX_FLAGS_RELEASE` or `CMAKE_CXX_FLAGS_DEBUG` globals
3. **Target-based properties**: All include dirs, compile features, and link libraries are per-target
4. **Module split**: Single `vartypes` library is now `vartypes-core` + `vartypes-widgets`
5. **Example gated**: Example is now behind `BUILD_EXAMPLES=OFF` by default
6. **JSON serialization**: Uses `QJsonDocument` from Qt Core (no additional Qt modules needed)

### Migration Steps
1. Replace `target_link_libraries(... vartypes)` with `target_link_libraries(... vartypes-core)` for non-GUI code
2. The `vartypes` interface target still works as a drop-in for code using both core and GUI
3. Include `VarJSON.h` for new JSON serialization support

## Qt6 Migration Notes

The current codebase is compatible with both Qt5 and Qt6. No additional migration is needed.

If building against Qt6 only, the following Qt5 compatibility patterns are already handled:
- `QT_VERSION_MAJOR` variable used throughout
- No Qt4-era patterns remain
- Qt imported targets used exclusively
