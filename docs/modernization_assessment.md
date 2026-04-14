# Modernization Assessment

## Current State

**vartypes** is a C++ object-oriented framework for hierarchical, thread-safe variables with XML serialization and Qt-based GUI editing. Licensed under GNU LGPL v3, authored by Stefan Zickler (2008).

### Build System
- CMake 3.16+, C++11, C11
- Qt5/Qt6 auto-detection via `find_package(QT NAMES Qt6 Qt5 ...)`
- Single flat `CMakeLists.txt` with one SHARED library target and one example executable
- Tests gated behind `BUILD_TESTS` option
- Global compiler flags (`CMAKE_CXX_FLAGS_RELEASE`), not target-based
- CI: Ubuntu (Qt5.15/Qt6.5), macOS (Qt6.5), Windows (Qt6.5)

### Dependencies
| Dependency | Status | Notes |
|---|---|---|
| Qt Core/Gui/Widgets | Required | Used throughout for signals, model/view, widgets |
| Boost | Optional dead code | Only in `VarAny.h` behind `VARTYPES_HAS_BOOST_SERIALIZATION` ifdef |
| Protobuf | Optional dead code | `VarProtoBuffer`/`VarAnyProtoObject` — protobuf include commented out |
| Custom xmlParser | Vendored (3175 LOC) | Third-party BSD parser by Frank Vanden Berghen, v2.23 |

### Architecture
| Module | Files | Description |
|---|---|---|
| Core infrastructure | `VarTypes.h`, `VarTypesInstance`, `VarTypesFactory`, `VarNotifier`, `VarBase64` | Singleton, factory, notification, base64 |
| Primitive values | `VarVal`, `VarIntVal`, `VarDoubleVal`, `VarBoolVal`, `VarStringVal` | Value-only classes (no Qt dependency except VarVal.h) |
| Full primitives | `VarType`, `VarInt`, `VarDouble`, `VarBool`, `VarString`, `VarBlob`, `VarList`, `VarSelection`, `VarStringEnum`, `VarTrigger`, `VarExternal`, `VarQWidget` | QObject-based, with GUI editor/XML methods |
| Optional wrappers | `VarAny`, `VarAnyVal`, `VarProtoBuffer`, `VarProtoBufferVal` | Generic pointer/protobuf wrappers |
| GUI | `VarItem`, `VarItemDelegate`, `VarTreeModel`, `VarTreeView`, `VarTreeViewOptions` | Qt Model/View tree editing |
| XML | `xmlParser.h/.cpp`, `VarXML.h/.cpp` | Serialization to/from XML files |
| Example | `example/src/mainwindow.h/.cpp`, `example.cpp` | Demo Qt GUI application |
| Tests | `test_varval.cpp`, `test_vartypes.cpp`, `test_xml.cpp` | Qt Test framework, basic coverage |

## Current Problems

### Critical
1. **Core/GUI coupling**: `VarType.h` includes `QPainter`, `QStyleOptionViewItem`, `QLineEdit`, `QApplication` — pure GUI headers in the base class used by everything
2. **Custom XML parser**: 3175 LOC of vendored third-party C code; unmaintained, potential security issues with untrusted input
3. **No serialization abstraction**: XML is the only format; deeply wired into `VarType::writeXML()`/`readXML()`
4. **Manual mutex patterns**: Raw `new QMutex()` in constructor, manual `lock()`/`unlock()` without RAII, error-prone
5. **Thread-unsafe singletons**: `VarTypesInstance` and `VarBase64` use raw `new` without any thread-safety

### High Priority
6. **C++11 standard too old**: Project uses C++11 but `std::make_shared`, `unique_ptr`, `QMutexLocker` are all available
7. **Global compiler flags**: `CMAKE_CXX_FLAGS_RELEASE` set globally instead of per-target
8. **No JSON support**: Modern configuration systems expect JSON; only XML exists
9. **No module separation**: Everything compiles into one monolithic shared library

### Medium Priority
10. **Boost dead code**: `VarAnyBoostSerializableObject` exists behind ifdef but is never used/tested
11. **Protobuf dead code**: `VarProtoBuffer` types exist but protobuf include is commented out
12. **`sprintf` usage**: `VarTypeImportExportOptions::getUniqueName()` uses `sprintf(cbuf, ...)` — buffer overflow risk
13. **`using namespace std;`** in header file (`VarType.h:198`) — namespace pollution for all includers
14. **No install export**: No `CMakePackageConfigHelpers` or `vartypesConfig.cmake` for downstream consumers

### Low Priority
15. **Example always builds**: No `BUILD_EXAMPLES` option
16. **Old README**: Plain text `README` file, not markdown
17. **Missing header guards consistency**: Some use `#ifndef X_H_`, others differ
18. **Dead commented-out code**: Significant blocks in `VarAny.h`, `VarProtoBuffer.h`

## Highest-Value Improvements

1. **Module split** — separate core from GUI so non-GUI users don't need Qt Widgets
2. **Serialization abstraction + JSON** — prepare for modern config formats
3. **Replace custom XML parser** — use Qt's `QXmlStreamReader`/`QXmlStreamWriter`
4. **RAII mutex patterns** — replace manual lock/unlock with `QMutexLocker`
5. **Modern CMake** — target-based properties, build options, install exports
6. **Remove Boost dependency** — clearly dead, just clean it up

## Risky Areas

1. **VarType.h GUI decoupling**: The base class has virtual methods `createEditor()`, `setEditorData()`, `setModelData()`, `getPaintValue()` that return Qt widgets. A full split would require a virtual interface or moving these to a separate class. **Strategy**: Keep virtual methods but make widget headers conditional on BUILD_GUI.
2. **xmlParser replacement**: VarXML and every VarType's `writeXML()`/`readXML()` use `XMLNode` directly. Replacing the parser requires touching all serialization paths. **Strategy**: Create an abstraction layer that wraps both old and new XML, then migrate.
3. **Thread safety macros**: The `VARTYPE_MACRO_LOCK`/`VARTYPE_MACRO_UNLOCK` macros are used pervasively. **Strategy**: Replace with `QMutexLocker` in-place, which is a safe mechanical transformation.

## Recommended Execution Order

1. Phase 0: Assessment (this document)
2. Phase 1: Build system modernization (CMake cleanup, options)
3. Phase 2: Dependency audit/cleanup (remove Boost dead code, isolate protobuf)
4. Phase 3: Module split (vartypes-core, vartypes-widgets)
5. Phase 4: Serialization modernization (abstraction, JSON backend, Qt XML backend)
6. Phase 5: Internal code quality (RAII mutexes, smart pointer singletons, sprintf→snprintf)
7. Phase 6: Tests (serialization round-trips, JSON tests, backward compat)
8. Phase 7: Documentation (README, migration notes, future work)
