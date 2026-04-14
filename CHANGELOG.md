# Changelog

All notable changes to VarTypes are documented in this file.

## v1.0.0 — 2026-04-15 (Modernization Release)

Major modernization of the VarTypes framework, bringing it up to date with modern C++, Qt 5/6,
and cross-platform CI/CD.

### Build System
- Migrated from legacy CMake/qmake to modern CMake 3.16+ with target-based configuration
- Split into modular library targets: `vartypes-core`, `vartypes-widgets`, and combined `vartypes`
- Added `BUILD_GUI`, `BUILD_EXAMPLES`, and `BUILD_TESTS` CMake options
- Added CMake package config for downstream `find_package(vartypes)` support
- Enabled `CMAKE_AUTOMOC` / `CMAKE_AUTORCC` for automatic Qt meta-object compilation

### Qt Compatibility
- Full support for both Qt 5 (>=5.15) and Qt 6
- Version-agnostic Qt references using `Qt${QT_VERSION_MAJOR}::`
- Removed deprecated Qt Xml module dependency (JSON uses `QJsonDocument` from Qt Core)

### Serialization
- **Added JSON serialization** (`VarJSON`) using `QJsonDocument` — recommended for new projects
- JSON read/write with hierarchy support, new node creation, and `getJSON()` / `setJSON()` API
- XML serialization (`VarXML`) remains fully supported for backward compatibility

### Code Modernization
- RAII mutex management with `unique_ptr<QMutex>`
- Replaced raw singletons with `unique_ptr`-based patterns
- `sprintf` → `snprintf` for buffer safety
- `NULL` → `nullptr` throughout
- Changed `emit` → `Q_EMIT` for macro safety

### Testing
- Added Qt Test-based unit test suites: `test_varval`, `test_vartypes`, `test_xml`, `test_json`
- Headless test support via `QT_QPA_PLATFORM=offscreen`

### CI/CD
- Added GitHub Actions CI workflow testing across Ubuntu, macOS, and Windows
- CI matrix covers Qt 5.15 and Qt 6.5 on multiple platforms
- Added explicit workflow permissions for CI security

### Documentation
- Comprehensive new README with quick start, build instructions, and platform matrix
- Added docs: build guide, serialization design, data migration notes, dependency audit,
  downstream integration, GrSim migration guide, modernization assessment, and future work

### Platform Support
- Ubuntu 22.04 / 24.04 (Qt 5.15, Qt 6.5)
- macOS 14 / 15 (Qt 6.5)
- Windows (Qt 6.5)

---

## v0.8.1 — 2018-12-10 (macOS / C++11 Compatibility)

Changes by [Mohammad Mahdi Rahimi (mahi97)](https://github.com/mahi97) to fix compilation on
macOS and modernize C++ standard usage.

- Replaced deprecated `<tr1/memory>` includes with `<memory>` (C++11 standard) across all headers
- Added C++11 standard flag to CMakeLists.txt
- Added `.gitignore`
- Migrated repository hosting to GitHub

---

## v0.8 — 2015-12-23

- Fixed Issue #2: `VarIntVal` constructor was checking `_val` before it was set
- Made `shared_ptr` explicitly use `std` namespace
- Cleanup in `VarTypesFactory`
- Added import/export options
- More coherent use of `shared_ptr`
- Added `VarAny` wrapper
- Added runtime loader interface for at-runtime loading of VarType `.so` files (optional)
- Changed `emit` to `Q_EMIT`
- Integrated example into `CMakeLists.txt` and removed `.pro` build file

## v0.7 — 2011-06-10

- Changed to `shared_ptr` model for all VarTypes
- VarTypes now requires the C++ extensions

## v0.6 — 2010-10-25

- Re-licensed from GPL to LGPL, so VarTypes can now be used in commercial applications
- Added CMake files for building shared library
- Removed directory prefixes from include directives to be compliant with CMake library installation
- Qt is now a requirement for compilation; removed all `VDATA_NO_QT` statements

## v0.5 — 2009-12-04

- VarTypes system now lives in the `VarTypes` namespace
- Renamed `VarData` class to `VarType`
- Restructured code to separate full `VarType` from raw data value `VarVal`, allowing better
  integration where pure high-performance data-types are needed without the overhead of mutexes
  and string labels
- VarTypes are now template-instantiated through `VarTypeTemplate`
- Added `VarTypesFactory` class for easier single-point creation of custom user-defined VarTypes
- Added `clone()` and `cloneDeep()` operators for all VarVals
- Added binary string store/load interface
- Renamed all enumerations using more reasonable conventions
- Changed `renderFlags` to `Flags` since they control more than just render behavior
- Improved string handling in XML I/O for code safety
- Compilation now works on Windows

## v0.4 — 2009-04-16

- Added multi-item selection type
- Added `VarNotifier` class to catch change-events for entire sub-trees
- Fixed several bugs
- Improved visualization:
  - Used persistent editors where applicable
  - Bools now based on checkboxes
  - Moved delegate functionality into respective type classes

## v0.3 — 2008-10-31

- Added string enums
- Fixed various performance bugs
- Added Doxygen-style headers to files and classes

## v0.2 — 2007-11-13

- Basic thread-safety added
- Added `VarQWidget` and `VarTrigger` (buttons) types
- Added persistent editor flag
- Added no-load and no-save flags

## v0.1 — 2007-09-27

- First release
