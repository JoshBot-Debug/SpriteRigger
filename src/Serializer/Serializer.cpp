#include "Serializer.h"

#include <cstring>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

Serializer::Serializer(const Options &options) : m_Options(options) {}

void Serializer::Stage(const std::string &key, const void *data,
                       uint64_t size) {

  const uint64_t chunkOffset = m_Chunks.size();

  // Write the manifest
  {
    size_t csize = m_Manifest.size();
    uint64_t ksize = static_cast<uint64_t>(key.size());

    m_Manifest.resize(csize + key.size() + sizeof(uint64_t) * 3);

    uint8_t *ptr = m_Manifest.data() + csize;

    // Write the key
    std::memcpy(ptr, &ksize, sizeof(uint64_t));
    ptr += sizeof(uint64_t);

    std::memcpy(ptr, key.c_str(), key.size());
    ptr += key.size();

    // Write the offset of the chunk
    std::memcpy(ptr, &chunkOffset, sizeof(uint64_t));
    ptr += sizeof(uint64_t);

    // Write the size of the chunk
    std::memcpy(ptr, &size, sizeof(uint64_t));
  }

  // Resize to accommodate the chunk
  m_Chunks.resize(chunkOffset + size);

  // Write the chunk
  std::memcpy(m_Chunks.data() + chunkOffset, data, size);
}

void Serializer::Write(const std::string &filepath) {
  std::ofstream file(filepath, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Failed to write data." << std::endl;
    return;
  }

  // Metadata
  {
    // Write the magic
    file.write(m_Options.magic.c_str(), m_Options.magic.size());

    // Write the version
    file.write(reinterpret_cast<const char *>(&m_Options.version),
               sizeof(uint32_t));

    uint64_t manifestSize = m_Manifest.size();
    uint64_t chunksSize = m_Chunks.size();

    // Write the manifest size
    file.write(reinterpret_cast<const char *>(&manifestSize), sizeof(uint64_t));

    // Write the chunks size
    file.write(reinterpret_cast<const char *>(&chunksSize), sizeof(uint64_t));
  }

  // Write the manifest
  file.write(reinterpret_cast<const char *>(m_Manifest.data()),
             m_Manifest.size());

  // Write the chunks
  file.write(reinterpret_cast<const char *>(m_Chunks.data()), m_Chunks.size());

  Clear();
}

void Serializer::Move(const std::string &from, const std::string &to) {
  try {
    std::filesystem::rename(from, to);
  } catch (std::filesystem::filesystem_error &e) {
    std::filesystem::remove(to);
    std::filesystem::rename(from, to);
  }

  Clear();
}

void Serializer::Load(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Failed to read file: " << filepath.data() << std::endl;
    return;
  }

  // Read magic
  std::string magic(m_Options.magic.size(), '\0');
  file.read(magic.data(), m_Options.magic.size());

  // Read version
  uint32_t version = 0;
  file.read(reinterpret_cast<char *>(&version), sizeof(uint32_t));

  // Read manifest size
  uint64_t manifestSize = 0;
  file.read(reinterpret_cast<char *>(&manifestSize), sizeof(uint64_t));

  // Read chunks size
  uint64_t chunksSize = 0;
  file.read(reinterpret_cast<char *>(&chunksSize), sizeof(uint64_t));

  std::cout << "Magic: " << magic << std::endl;
  std::cout << "Version: " << version << std::endl;
  std::cout << "Manifest size: " << manifestSize << std::endl;
  std::cout << "Chunks   size: " << chunksSize << std::endl;

  assert(magic == m_Options.magic);

  /// TODO: Handle versions
  assert(version == m_Options.version);

  if (manifestSize == 0)
    return;

  m_Manifest.resize(manifestSize);
  m_Chunks.resize(chunksSize);

  file.read(reinterpret_cast<char *>(m_Manifest.data()), manifestSize);
  file.read(reinterpret_cast<char *>(m_Chunks.data()), chunksSize);
}

std::vector<uint8_t> Serializer::Get(const std::string &key) {
  std::vector<uint8_t> result;

  for (size_t i = 0; i < m_Manifest.size();) {
    uint64_t kSize = 0;

    uint8_t *ptr = m_Manifest.data() + i;

    std::memcpy(&kSize, ptr, sizeof(uint64_t));
    ptr += sizeof(uint64_t);

    std::string k("", kSize);

    std::memcpy(k.data(), ptr, kSize);
    ptr += kSize;

    if (k == key) {
      uint64_t chunkOffset = 0;
      uint64_t chunkSize = 0;

      std::memcpy(&chunkOffset, ptr, sizeof(uint64_t));
      ptr += sizeof(uint64_t);

      std::memcpy(&chunkSize, ptr, sizeof(uint64_t));

      if (m_Chunks.size() < chunkOffset + chunkSize)
        throw std::runtime_error("Invalid file. The chunks don't even out.");

      size_t rSize = result.size();
      result.resize(rSize + chunkSize);
      std::memcpy(result.data() + rSize, m_Chunks.data() + chunkOffset,
                  chunkSize);
      break;
    }

    i += kSize + sizeof(uint64_t) * 3;
  }

  return result;
}

std::vector<std::vector<uint8_t>> Serializer::GetAll(const std::string &key) {
  std::vector<std::vector<uint8_t>> results;

  for (size_t i = 0; i < m_Manifest.size();) {
    uint64_t kSize = 0;

    uint8_t *ptr = m_Manifest.data() + i;

    std::memcpy(&kSize, ptr, sizeof(uint64_t));
    ptr += sizeof(uint64_t);

    std::string k("", kSize);

    std::memcpy(k.data(), ptr, kSize);
    ptr += kSize;

    if (k == key) {
      uint64_t chunkOffset = 0;
      uint64_t chunkSize = 0;

      std::memcpy(&chunkOffset, ptr, sizeof(uint64_t));
      ptr += sizeof(uint64_t);

      std::memcpy(&chunkSize, ptr, sizeof(uint64_t));

      if (m_Chunks.size() < chunkOffset + chunkSize)
        throw std::runtime_error("Invalid file. The chunks don't even out.");

      size_t rSize = results.size();
      results.resize(rSize + 1);
      results[rSize].resize(chunkSize);
      std::memcpy(results[rSize].data(), m_Chunks.data() + chunkOffset,
                  chunkSize);
    }

    i += kSize + sizeof(uint64_t) * 3;
  }

  return results;
}

void Serializer::Clear() {
  m_Manifest.clear();
  m_Chunks.clear();
}
