#pragma once

#include <map>
#include <string>
#include <vector>

struct SaveFile
{
  std::string name;
  std::string extension;
  std::string directory;

  SaveFile() {};
  SaveFile(std::string name, std::string extension, std::string directory) : name(name), extension(extension), directory(directory) {};
  SaveFile(std::string filePath);

  void set(std::string name, std::string extension, std::string directory);
  void set(std::string filePath);

  operator std::string() const;
};

class StateSerializer
{
private:
  SaveFile saveFile;

  std::map<std::string, std::map<std::string, std::string>> mMap;
  std::map<std::string, std::vector<std::string>> mVec;

public:
  StateSerializer() {};
  StateSerializer(SaveFile saveFile);
  ~StateSerializer() = default;

  void setSaveFile(SaveFile saveFile);
  void setSaveFileName(std::string name);
  void setSaveFileExtension(std::string extension);
  void setSaveFileDirectory(std::string directory);

  SaveFile *getSaveFile();
  std::string *getSaveFileName();
  std::string *getSaveFileExtension();
  std::string *getSaveFileDirectory();

  std::map<std::string, std::string> *map(const char *key);
  std::vector<std::string> *vector(const char *key);

  bool write();
  bool write(SaveFile saveFile);
  bool write(std::string filePath);

  bool read();
  bool read(SaveFile saveFile);
  bool read(std::string filePath);
};