#pragma once

#include <string>
#include <vector>

class Serializer {
public:
  struct Options {
    std::string magic;
    uint32_t version = 0;
  };

  enum class Type : uint8_t {
    RAW = 0,
  };

private:
  Options m_Options;

  std::vector<uint8_t> m_Manifest = {};

  std::vector<uint8_t> m_Chunks = {};

private:
  void WriteBytes(std::vector<uint8_t> &buffer, const void *data,
                  uint64_t size);

public:
  Serializer(const Options &options);

  /**
   * Prepares the manifest and chunks
   */
  void Stage(const std::string &name, const Type &type, const void *data,
             uint64_t size);

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
  void Load(const std::string &filepath);
};