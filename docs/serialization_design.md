# Serialization Design

## Overview

The vartypes library supports two serialization formats:
- **XML** (legacy, backward-compatible)
- **JSON** (modern, recommended for new projects)

Both formats serialize the same VarType tree structure with equivalent semantics.

## Architecture

### XML Backend (`VarXML`)
- Uses the vendored `xmlParser` library (third-party, BSD licensed)
- Each VarType implements `writeXML()` and `readXML()` virtual methods
- Deeply integrated with the VarType class hierarchy
- Supports external blob storage, external XML file references
- Root element: `<VarXML>...</VarXML>`

### JSON Backend (`VarJSON`)
- Uses Qt's `QJsonDocument`/`QJsonObject`/`QJsonArray`
- Static helper class with `write()`, `read()`, `getJSON()` methods
- Each VarType node becomes a JSON object: `{ "type": "...", "name": "...", "value": "...", "children": [...] }`
- Root structure: `{ "VarJSON": [ ... ] }`

## Format Comparison

### XML Format
```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<VarXML>
  <VarList name="root">
    <VarInt name="my_int">42</VarInt>
    <VarDouble name="my_double" minval="0" maxval="100">3.14</VarDouble>
    <VarBool name="my_bool">true</VarBool>
    <VarString name="my_string">hello</VarString>
  </VarList>
</VarXML>
```

### JSON Format
```json
{
  "VarJSON": [
    {
      "type": "VarList",
      "name": "root",
      "value": "",
      "children": [
        { "type": "VarInt", "name": "my_int", "value": "42" },
        {
          "type": "VarDouble",
          "name": "my_double",
          "value": "3.140000",
          "attributes": { "minval": 0, "maxval": 100 }
        },
        { "type": "VarBool", "name": "my_bool", "value": "true" },
        { "type": "VarString", "name": "my_string", "value": "hello" }
      ]
    }
  ]
}
```

## API Reference

### VarXML (unchanged from legacy)
```cpp
// Write
VarXML::write(world, "settings.xml");
VarXML::write(world, "settings.xml", options);

// Read (merges with existing tree)
world = VarXML::read(world, "settings.xml");
world = VarXML::read(world, "settings.xml", options);

// Get XML string
std::string xml = VarXML::getXML(world);
```

### VarJSON (new)
```cpp
// Write
VarJSON::write(world, "settings.json");

// Read (merges with existing tree)
world = VarJSON::read(world, "settings.json");

// Get JSON string
std::string json = VarJSON::getJSON(world);
```

## Design Decisions

1. **Value storage as strings**: Both XML and JSON store values using `getSerialString()`/`setSerialString()`, ensuring consistent representation across formats.

2. **Type information preserved**: Both formats store the registered type name (e.g., "VarInt", "VarDouble") to enable factory-based reconstruction.

3. **Merge semantics**: Both `read()` methods merge loaded data with an existing tree by matching node names and types, preserving the "update existing, append new" pattern.

4. **JSON attributes**: Min/max metadata is stored in an optional `"attributes"` object, separate from the value, similar to XML attribute semantics.

## Limitations

### JSON Backend
- External blob storage (`VARTYPE_FLAG_SERIALIZE_EXTERNALLY`) is not yet supported in the JSON backend
- External XML node references (`VARTYPE_FLAG_EXTERNAL_XML`) are not supported in JSON
- Min/max attributes are written to JSON for documentation purposes but are not restored on read (the generic `VarPtr` interface does not expose `setMin`/`setMax`; constraints are set at tree construction time by the application)
- These features require additional implementation documented in future_work.md

### XML Backend
- Uses the vendored xmlParser which is unmaintained
- Character encoding handling may have edge cases with non-ASCII content
- The parser is not suitable for processing untrusted XML input

## Future Work

1. **Serialization interface**: Create a formal `ISerializer` interface that both XML and JSON backends implement
2. **Replace xmlParser**: Migrate XML backend to Qt's `QXmlStreamReader`/`QXmlStreamWriter`
3. **External storage in JSON**: Implement blob and external reference support
4. **Schema validation**: Add optional JSON Schema validation
