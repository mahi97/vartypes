# VarTypes

An object-oriented, thread-safe C++/Qt framework for managing hierarchical variables with serialization (XML and JSON) and optional GUI tree editing.

## Features

- **Hierarchical variable system**: Organize variables in trees with named nodes
- **Built-in types**: Int, Double, Bool, String, Blob, List, Selection, StringEnum, Trigger, External
- **Thread-safe**: Per-variable mutex protection with RAII patterns
- **XML serialization**: Read/write configuration trees to XML files (legacy format, fully supported)
- **JSON serialization**: Read/write configuration trees to JSON files (modern format, recommended)
- **Qt Model/View GUI**: Tree widget for browsing and editing variables interactively
- **Factory system**: Register custom types and create them by name at runtime
- **Signal/slot integration**: Qt signals for change notification and event handling

## Quick Start

```cpp
#include "VarTypes.h"
#include "VarInt.h"
#include "VarDouble.h"
#include "VarList.h"
#include "VarJSON.h"

using namespace VarTypes;

// Create a variable tree
VarListPtr root(new VarList("settings"));
root->addChild(VarIntPtr(new VarInt("width", 1920, 640, 3840)));
root->addChild(VarDoublePtr(new VarDouble("scale", 1.0, 0.1, 10.0)));

std::vector<VarPtr> world = { root };

// Save to JSON
VarJSON::write(world, "config.json");

// Load from JSON (merges with existing tree)
world = VarJSON::read(world, "config.json");
```

## Building

```bash
# Prerequisites: CMake 3.16+, Qt 5.15+ or Qt 6.5+, C++14 compiler

# Basic build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# With tests and examples
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
cmake --build build --parallel

# Run tests
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
```

### Build Options

| Option | Default | Description |
|---|---|---|
| `BUILD_GUI` | ON | Build the `vartypes-widgets` GUI library |
| `BUILD_EXAMPLES` | OFF | Build example applications |
| `BUILD_TESTS` | OFF | Build unit tests |
| `BUILD_STATIC` | OFF | Build static libraries instead of shared |

## Library Modules

| Module | Target | Description |
|---|---|---|
| Core | `vartypes-core` | Variable system, serialization, factory |
| Widgets | `vartypes-widgets` | Qt tree view, model, delegate, item |
| Combined | `vartypes` | Interface target linking both modules |

### Linking

```cmake
# Non-GUI usage (core only)
target_link_libraries(my_app PRIVATE vartypes-core)

# GUI usage (core + widgets)
target_link_libraries(my_app PRIVATE vartypes)
```

## Serialization

### JSON (recommended for new projects)
```cpp
#include "VarJSON.h"

VarJSON::write(world, "config.json");
world = VarJSON::read(world, "config.json");
std::string json = VarJSON::getJSON(world);
```

### XML (legacy, fully backward-compatible)
```cpp
#include "VarXML.h"

VarXML::write(world, "config.xml");
world = VarXML::read(world, "config.xml");
std::string xml = VarXML::getXML(world);
```

## Documentation

- [Changelog](CHANGELOG.md)
- [Build and Porting Guide](docs/build_and_porting.md)
- [Serialization Design](docs/serialization_design.md)
- [Data Migration Notes](docs/data_migration_notes.md)
- [Dependency Audit](docs/dependency_audit.md)
- [Modernization Assessment](docs/modernization_assessment.md)
- [Future Work](docs/future_work.md)

## Platform Support

| Platform | Qt 5.15 | Qt 6.5 |
|---|---|---|
| Ubuntu 22.04 | ✅ | ✅ |
| Ubuntu 24.04 | ✅ | ✅ |
| macOS 14/15 (arm64) | — | ✅ |
| Windows | ✅ | ✅ |

## Authors

- **Stefan Zickler** — Original author (2007–2015) — [szickler.net](http://szickler.net)
- **Mohammad Mahdi Rahimi (mahi97)** — macOS/C++11 fixes, modernization (2018–present) — [github.com/mahi97](https://github.com/mahi97)

## License

GNU Lesser General Public License Version 3 (LGPL v3). See [COPYING](COPYING).
