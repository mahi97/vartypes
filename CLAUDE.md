# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

VarTypes is a C++/Qt framework for managing typed variables with hierarchical organization, thread-safety, XML and JSON serialization, and a Qt-based GUI (tree view editor). It builds as two shared libraries (`libvartypes-core` + `libvartypes-widgets`) and is licensed under LGPL v3.

## Build Commands

```bash
# Full build (Release)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel

# Build with tests and examples
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
cmake --build build --config Release --parallel

# Core-only build (no tree view widget library, but still needs Qt Widgets)
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=OFF
cmake --build build --config Release --parallel

# Run tests (requires -DBUILD_TESTS=ON)
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure -C Release

# Windows build (no CMAKE_BUILD_TYPE needed for multi-config generators)
cmake -B build -DBUILD_TESTS=ON
cmake --build build --config Release --parallel

# Install (generates CMake package config for find_package(vartypes))
cmake --install build --prefix /usr/local
```

Tests require `QT_QPA_PLATFORM=offscreen` since they use Qt GUI types without a display server.

### Build Options

| Option | Default | Description |
|---|---|---|
| `BUILD_GUI` | ON | Build `vartypes-widgets` GUI library |
| `BUILD_EXAMPLES` | OFF | Build example applications (requires `BUILD_GUI=ON`) |
| `BUILD_TESTS` | OFF | Build unit tests |
| `BUILD_STATIC` | OFF | Build static libraries instead of shared |

## Qt Compatibility

The build system supports both Qt5 (>=5.15) and Qt6. CMake auto-detects Qt6 first, falling back to Qt5. All Qt references use `Qt${QT_VERSION_MAJOR}::` to stay version-agnostic. JSON serialization uses `QJsonDocument` from Qt Core (no additional Qt modules needed). The CI matrix tests against Qt 5.15.2 and Qt 6.5.3 across Ubuntu, macOS, and Windows.

## Architecture

Everything lives in the `VarTypes` namespace.

### Library Targets

| Target | Description |
|---|---|
| `vartypes-core` | Types, serialization (XML + JSON), factory, primitives |
| `vartypes-widgets` | Qt tree view, model, delegate (only built when `BUILD_GUI=ON`) |
| `vartypes` | INTERFACE target linking both core and widgets |

**Important**: `vartypes-core` still depends on `Qt::Gui` and `Qt::Widgets` because the `VarType` base class has virtual GUI methods (`createEditor`, `paint`, etc.). A full decoupling would require an interface refactor (documented in `docs/future_work.md`).

### Layer Structure

1. **Value Layer** (`vartypes/primitives/Var*Val.h/.cpp`) - Lightweight data holders (`VarIntVal`, `VarDoubleVal`, `VarStringVal`, `VarBoolVal`, `VarAnyVal`). Pure value types.

2. **Type Layer** (`vartypes/primitives/Var*.h/.cpp`) - Full VarType nodes inheriting both `VarType` and the corresponding `VarVal` via `VarTypeTemplate<T>`. These add: string name, flags (`VarTypeFlag`), thread-safe locking via `unique_ptr<QMutex>`, Qt signals (`hasChanged`, `wasEdited`, `XMLwasRead`, `XMLwasWritten`), serialization, and GUI editor support.

3. **GUI Layer** (`vartypes/gui/`) - Qt Model/View implementation: `VarTreeModel`, `VarTreeView`, `VarItem`, `VarItemDelegate`.

4. **Serialization** - XML: `VarXML` + vendored `xmlParser` (BSD). JSON: `VarJSON` using `QJsonDocument`. Both use `getSerialString()`/`setSerialString()` for format equivalence.

5. **Factory/Instance** - `VarTypesFactory` maps type-name strings to constructors. `VarTypesInstance` is a singleton wrapper.

### Key Patterns

- All VarType nodes are `std::shared_ptr<VarType>` (aliased as `VarPtr`).
- Thread safety uses `unique_ptr<QMutex>` with RAII lock patterns.
- `VarList` is the container node for trees. `VarExternal` serializes subtrees to separate XML files.
- `VarTypeFlag` bitflags: `READONLY`, `HIDDEN`, `PERSISTENT`, `NOSAVE`, `NOLOAD`, `AUTO_EXPAND`, etc.
- Recursive search: `findChildRecursive("Globals/Child/Setting")`.

## Tests

Four test suites in `tests/` using Qt Test framework:
- `test_varval` - Primitive value types
- `test_vartypes` - VarType nodes, factory, min/max clamping
- `test_xml` - XML round-trip serialization
- `test_json` - JSON round-trip serialization, hierarchy, new node creation

## Downstream Consumption

```cmake
# After install:
find_package(vartypes REQUIRED)
target_link_libraries(my_app PRIVATE vartypes::vartypes)

# Or as subdirectory:
add_subdirectory(vartypes)
target_link_libraries(my_app PRIVATE vartypes)
```

See `docs/downstream_integration.md` and `docs/grsim_migration.md` for details.

## Example

`example/src/mainwindow.cpp` demonstrates building a VarType tree and displaying it in a `VarTreeView`. Build with `-DBUILD_EXAMPLES=ON -DBUILD_GUI=ON`.
