#include "Serializer.h"

#include <cstring>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void Serializer::WriteBytes(std::vector<uint8_t> &buffer, const void *data,
                            uint64_t size) {
  // Resize accommodate the new data
  buffer.resize(buffer.size() + size + sizeof(uint64_t));

  // Write the size of the data
  std::memcpy(buffer.data(), &size, sizeof(uint64_t));

  // Write the data to the buffer
  std::memcpy(buffer.data(), data, size);
}

Serializer::Serializer(const Options &options) : m_Options(options) {}

void Serializer::Stage(const std::string &name, const Type &type,
                       const void *data, uint64_t size) {

  // Write the manifest
  {
    // Write the name
    WriteBytes(m_Manifest, name.data(), static_cast<uint64_t>(name.size()));
    // Write the type
    WriteBytes(m_Manifest, &type, static_cast<uint64_t>(sizeof(type)));

    const uint64_t chunkOffset = m_Chunks.size();

    // Write the offset of the chunk
    WriteBytes(m_Manifest, &chunkOffset,
               static_cast<uint64_t>(sizeof(chunkOffset)));
    // Write the size of the chunk
    WriteBytes(m_Manifest, &size, static_cast<uint64_t>(sizeof(size)));
  }

  // Resize to accommodate the chunk
  m_Chunks.resize(m_Chunks.size() + size);

  // Write the chunk
  std::memcpy(m_Chunks.data(), data, size);
}

void Serializer::Write(const std::string &tempfile) {
  std::ofstream file(tempfile, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Failed to save data." << std::endl;
    return;
  }

  // Write the magic
  file.write(m_Options.magic.data(), m_Options.magic.size());

  // Write the version
  file.write(reinterpret_cast<const char *>(&m_Options.version),
             sizeof(uint32_t));

  // Write the manifest
  file.write(reinterpret_cast<const char *>(m_Manifest.data()),
             m_Manifest.size());

  // Write the chunks
  file.write(reinterpret_cast<const char *>(m_Chunks.data()), m_Chunks.size());

  file.close();
}

void Serializer::Save(const std::string &tempfile,
                      const std::string &savefile) {
  try {
    std::filesystem::rename(tempfile, savefile);
  } catch (std::filesystem::filesystem_error &e) {
    std::filesystem::remove(savefile);
    std::filesystem::rename(tempfile, savefile);
  }
}

void Serializer::Load(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Failed to read save file: " << filepath.data() << std::endl;
    return;
  }

  // Read magic
  std::string magic(m_Options.magic.size(), '\0');
  file.read(magic.data(), m_Options.magic.size());

  // Read version
  uint32_t version = 0;
  file.read(reinterpret_cast<char *>(&version), sizeof(uint32_t));

  std::cout << "Magic: " << magic << std::endl;
  std::cout << "Version: " << version << std::endl;

  assert(magic == m_Options.magic);

  /// TODO: Handle versions
  assert(version == m_Options.version);

  file.close();
}
