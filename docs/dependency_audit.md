# Dependency Audit

## Required Dependencies

### Qt (5.15+ or 6.5+)
- **Qt Core**: Used throughout for QObject, signals/slots, QString, QFile, etc.
- **Qt Gui**: QPainter, QStyleOptionViewItem (used in VarType base class for paint/editor methods)
- **Qt Widgets**: QLineEdit, QSpinBox, QCheckBox, QComboBox, QPushButton (used in VarType editor creation, GUI tree view)
- **Qt Xml**: Used by VarJSON for QJsonDocument (JSON serialization backend)

**Note**: Qt Gui and Qt Widgets are still used in the core library because `VarType` base class has virtual GUI methods (`createEditor`, `setEditorData`, `setModelData`, `paint`). A complete decoupling would require an interface-based split, which is documented as future work.

### C++ Standard Library
- `<memory>`: `std::shared_ptr`, `std::unique_ptr`, `std::make_unique`
- `<string>`, `<vector>`, `<map>`, `<set>`, `<queue>`, `<deque>`
- `<fstream>`, `<sstream>`, `<iostream>`

## Optional Dependencies

### Boost Serialization
- **Status**: Dead code, kept behind `#ifdef VARTYPES_HAS_BOOST_SERIALIZATION`
- **Usage**: Only in `VarAny.h` for `VarAnyBoostSerializableObject<T>` template
- **Recommendation**: This define is never set by the build system. The code is preserved for backward compatibility with downstream projects that may define it manually. No Boost packages are required to build.

### Google Protobuf
- **Status**: Vestigial, includes commented out
- **Usage**: `VarProtoBuffer.h`, `VarProtoBufferVal.h`, `VarAnyProtoObject<T>`
- **Recommendation**: The protobuf headers are commented out in the source. The types compile but have no actual protobuf dependency. Kept for API compatibility.

## Vendored Dependencies

### xmlParser (Frank Vanden Berghen, v2.23)
- **Location**: `vartypes/xml/xmlParser.h` and `xmlParser.cpp`
- **Size**: 3,175 lines of code
- **License**: BSD
- **Status**: Self-contained C/C++ XML parser, no external dependency
- **Usage**: Used by VarXML for all XML serialization and by VarBase64 for Base64 encoding
- **Risk**: Unmaintained third-party code. Not recommended for untrusted XML input.
- **Recommendation**: Retained for XML backward compatibility. JSON serialization (using Qt's QJsonDocument) is the recommended modern alternative. Future work could replace xmlParser with Qt's QXmlStreamReader/Writer.

## Dependency Changes in This Modernization

| Change | Rationale |
|---|---|
| Added `Qt::Xml` module | Required for QJsonDocument (JSON serialization) |
| C++17 required | `std::make_unique`, structured bindings potential, better STL |
| Boost not required | Was never truly required; only behind opt-in `#ifdef` |
| Protobuf not required | Was never truly required; includes are commented out |

## GUI/Core Coupling Analysis

The `VarType` base class (in `primitives/VarType.h`) includes:
- `QPainter`, `QStyleOptionViewItem`, `QLineEdit` headers
- Virtual methods: `createEditor()`, `setEditorData()`, `setModelData()`, `paint()`, `sizeHint()`

This means `vartypes-core` still depends on Qt Gui and Qt Widgets. A complete separation would require:
1. Moving GUI virtual methods to a separate interface class
2. Using a plugin/adapter pattern for editor creation
3. This is documented as future work

The `vartypes-widgets` library (tree view, model, delegate, item) is properly separated and only contains GUI-specific code.
