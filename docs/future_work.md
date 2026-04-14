# Future Work

## High Priority

### 1. Complete GUI/Core Decoupling
The `VarType` base class still includes Qt Gui headers (`QPainter`, `QStyleOptionViewItem`, `QLineEdit`) and has virtual GUI methods (`createEditor`, `setEditorData`, `setModelData`, `paint`, `sizeHint`).

**Recommended approach:**
- Create a `VarTypeEditorInterface` abstract class in the widgets module
- Move GUI virtual methods to this interface
- Use a registration/adapter pattern so core types don't need GUI headers
- This would allow `vartypes-core` to drop Qt Gui/Widgets dependencies entirely

### 2. Replace vendored xmlParser
The custom xmlParser (3,175 LOC, Frank Vanden Berghen v2.23) is unmaintained and represents a security risk for untrusted input.

**Recommended approach:**
- Replace with Qt's `QXmlStreamReader`/`QXmlStreamWriter`
- Maintain the same XML format for backward compatibility
- VarXML would call Qt XML APIs instead of xmlParser APIs
- Base64 encoding could use `QByteArray::toBase64()`/`fromBase64()`

### 3. Formal Serialization Interface
Create an `ISerializer` interface:
```cpp
class ISerializer {
public:
    virtual void write(const std::vector<VarPtr>&, const std::string& filename) = 0;
    virtual std::vector<VarPtr> read(std::vector<VarPtr>, const std::string& filename) = 0;
    virtual std::string serialize(const std::vector<VarPtr>&) = 0;
};
```

### 4. JSON External Storage Support
Add support for:
- External blob files (matching XML's `ExternalBlob` feature)
- Split configuration files (matching XML's `ExternalXMLNode`)

## Medium Priority

### 5. Thread-Safe Singletons
`VarTypesInstance` and `VarBase64` singletons are not thread-safe. Use `std::call_once` or C++11 Meyers singleton pattern:
```cpp
static VarTypesFactory& getFactory() {
    static VarTypesFactory instance;
    return instance;
}
```

### 6. Remove `using namespace std;` from Headers
`VarType.h` (line 198) and `VarVal.h` (line 25) contain `using namespace std;` which pollutes the global namespace for all includers. This should be removed and all `string`/`vector` usages qualified with `std::`.

### 7. Clean Up Dead Code
- Remove commented-out code blocks in `VarAny.h` (lines 43-73, 116-128)
- Remove commented-out code blocks in various headers
- Clean up `VarProtoBuffer.h` commented-out protobuf includes

### 8. CMake Package Config
Add proper `vartypesConfig.cmake` generation so downstream projects can use:
```cmake
find_package(vartypes REQUIRED)
```

## Low Priority

### 9. Namespace Cleanup
Consider renaming namespace to `vartypes` (lowercase) to match modern C++ conventions.

### 10. Documentation
- Add Doxygen generation to CMake
- Generate API reference documentation
- Add more inline documentation

### 11. Additional Serialization Formats
While JSON is the recommended modern format, the serialization abstraction could support:
- YAML (for human-editable configs)
- TOML (for simple key-value configs)
- Binary format (for performance-critical use cases)

### 12. Comprehensive Test Suite
Current tests cover basic functionality. Additional tests needed:
- Thread safety tests (concurrent read/write)
- Stress tests (large trees)
- Edge cases (empty values, special characters, Unicode)
- GUI tests (if QTest supports it)
- External storage tests
