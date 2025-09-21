#pragma once

#include <string>
#include <vector>
#include <stdint.h>

class Serializer {
public:
  struct Options {
    std::string magic;
    uint32_t version = 0;
  };

private:
  Options m_Options;

  std::vector<uint8_t> m_Manifest = {};

  std::vector<uint8_t> m_Chunks = {};

public:
  Serializer(const Options &options);

  /**
   * Prepares the manifest and chunks
   */
  void Stage(const std::string &key, const void *data, uint64_t size);

  /**
   * Writes to a file
   */
  void Write(const std::string &filepath);

  /**
   * Moves a file
   */
  void Move(const std::string &from, const std::string &to);

  /**
   * Loads the save file to memory
   */
  bool Load(const std::string &filepath);

  /**
   * Get data
   */
  std::vector<uint8_t> Get(const std::string &key);
  std::vector<std::vector<uint8_t>> GetAll(const std::string &key);

  /**
   * Clears all chunks and manifest data.
   */
  void Clear();
};