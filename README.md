# Sprite Rigger

A **sprite rigger & animator** desktop application.

---

## Setup (Linux)

Clone the project
```bash
git clone https://github.com/JoshBot-Debug/SpriteRigger.git
cd SpriteRigger
```

Initialize all submodules
```bash
git submodule update --init --recursive
cd vendor/imgui
git checkout docking
cd ../..
```
Install dependencies like glm, opengl
```bash
sudo apt update
sudo apt install \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  mesa-common-dev \
  libglfw3-dev \
  libopengl-dev \
  libglx-dev \
  libglm-dev \
  libstb-dev
```

Build the project
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build
```

Run the project
```bash
./build/SpriteRigger
```

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
- [x] **Header**: Include magic string + version for compatibility checks.
- [x] **Manifest**: Store `{ name, type, offset, size }` for all chunks.
- [ ] **Future versions**: Allow migration between file format versions.
- [x] **Atomic save**: Write to `.tmp` file, then rename on success.

---

## Roadmap

- [x] Implement serializer/deserializer
- [ ] Add compression/decompression layer
- [ ] Add checksum per chunk for corruption detection
- [ ] Support incremental save if necessary
- [ ] Need to add Deferred deletion
- [ ] Animation system store a raw pointer, this may pose a problem, may want to use Weak Handles (Generation IDs)
- [ ] Add light theme support

---

## Known issues
- [ ] There is an issue, when you create a bone save the file, then open a new file and hit Ctr+S, you get an out-of-bounds index. 
This is because the `GetEntityTypeId<E>()` in Registry.h is static. When the project reopens, since the program did not stop, all global statics remain cached. I don't want to use **RTTI** to keep track of entity types so this is pending until I find a better solution
