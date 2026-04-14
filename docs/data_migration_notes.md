# Data Migration Notes

## XML to JSON Migration

### Automatic Conversion

To convert existing XML configuration files to JSON format:

```cpp
#include "VarXML.h"
#include "VarJSON.h"

// Load from XML
std::vector<VarPtr> world;
// Set up your VarType tree structure...
world = VarXML::read(world, "old_settings.xml");

// Save as JSON
VarJSON::write(world, "new_settings.json");
```

### Manual Conversion

The formats are structurally equivalent. An XML node:
```xml
<VarInt name="my_int" minval="0" maxval="100">42</VarInt>
```

Maps to JSON:
```json
{
  "type": "VarInt",
  "name": "my_int",
  "value": "42",
  "attributes": { "minval": 0, "maxval": 100 }
}
```

### Known Differences

1. **Floating point precision**: XML and JSON may format floating-point values slightly differently due to `getSerialString()` using `sprintf("%lf", val)`. Both formats use the same underlying string, so round-trip should be identical.

2. **Attribute handling**: In XML, min/max are stored as XML attributes (`minval="0" maxval="100"`). In JSON, they are in an `"attributes"` sub-object. The JSON backend currently reads but does not write attribute-driven constraints — values are set through the serial string interface instead.

3. **External blobs**: XML supports external blob storage (`ExternalBlob` nodes). The JSON backend does not yet support this feature. Files using external blobs should continue using XML format.

4. **External XML references**: XML supports `ExternalXMLNode` references to split configuration across multiple files. This is not supported in JSON format.

## Backward Compatibility

- **XML read/write is fully preserved**: All existing XML files continue to work
- **API unchanged**: `VarXML::read()` and `VarXML::write()` have the same signatures
- **JSON is additive**: New `VarJSON` class provides parallel JSON support
- **No breaking changes**: No existing serialization behavior is modified

## Recommended Migration Strategy

1. **New projects**: Use `VarJSON` for all serialization
2. **Existing projects**: Continue using `VarXML` for compatibility
3. **Gradual migration**: Load with `VarXML::read()`, save with both `VarXML::write()` and `VarJSON::write()` to maintain both formats during transition
4. **Full migration**: Once all consumers support JSON, switch to `VarJSON::read()` exclusively
