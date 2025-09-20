# Sprite Rigger

A **sprite rigger & animator** desktop application.

---

## Save File Format (`.sprig`)

The project state is stored in a single container file with extension `.sprig`.

### File Layout

```
filename.sprig
┌───────────────────────────────────────────────┐
│ Header                                        │
│   - magic         (x bytes)                   │
│   - version       (uint32_t)                  │
│   - manifestSize  (uint64_t)                  │
│   - chunksSize    (uint64_t)                  │
├───────────────────────────────────────────────┤
│ Manifest (binary)                             │
│   - entries: { key, offset, size }            │
├───────────────────────────────────────────────┤
│ Chunk 1 (raw/compressed bytes)                │
├───────────────────────────────────────────────┤
│ Chunk 2 (raw/compressed bytes)                │
├───────────────────────────────────────────────┤
│ ...                                           │
└───────────────────────────────────────────────┘
```

- **Header**
  - `magic`: identifies the file type (e.g., `"SPRIG\0\0"`).
  - `version`: save file format version.
  - `manifestSize`: size of the manifest block in bytes.

- **Manifest**
  Describes all chunks in the file. Each entry includes:
  - `name`: logical name of the chunk (e.g., `"settings"`, `"svo"`)
  - `type`: data type (e.g., `json`, `binary`)
  - `offset`: byte offset from start of file
  - `size`: length of the chunk in bytes

- **Chunks**
  Contain the actual serialized data. Each chunk may be stored raw or compressed.

---

## Serializer TODO

- [ ] **Compression**: Add per-chunk compression (LZ4/zlib).
- [ ] **Header**: Include magic string + version for compatibility checks.
- [ ] **Manifest**: Store `{ name, type, offset, size }` for all chunks.
- [ ] **Future versions**: Allow migration between file format versions.
- [ ] **Atomic save**: Write to `.tmp` file, then rename on success.

---

## Roadmap

- [ ] Implement serializer/deserializer
- [ ] Add compression/decompression layer
- [ ] Add checksum per chunk for corruption detection
- [ ] Support incremental save if necessary

---