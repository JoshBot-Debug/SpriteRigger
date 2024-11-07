#include "stateSerializer.h"

#include <filesystem>
#include <iostream>
#include <fstream>

StateSerializer::StateSerializer(SaveFile saveFile) : saveFile(saveFile) {}

void StateSerializer::setSaveFileName(std::string name)
{
  this->saveFile.name = name;
}

void StateSerializer::setSaveFileExtension(std::string extension)
{
  this->saveFile.extension = "." + extension;
}

void StateSerializer::setSaveFileDirectory(std::string directory)
{
  this->saveFile.directory = directory;
}

std::map<std::string, std::string> *StateSerializer::getMap()
{
  return &this->map;
}

std::vector<std::string> *StateSerializer::getVector()
{
  return &this->vector;
}

bool StateSerializer::write()
{
  return this->write(this->saveFile);
}

bool StateSerializer::write(SaveFile saveFile)
{
  std::filesystem::path filepath = std::filesystem::path(saveFile.directory) / (saveFile.name + saveFile.extension);

  std::ofstream out(filepath, std::ios::binary);
  if (out)
  {
    if (!out.is_open())
      return false;

    /**
     * Save the size of the vector as the first size_t byte
     * We use this later to read data into the vector
     */
    size_t vSize = this->vector.size();
    out.write(reinterpret_cast<const char *>(&vSize), sizeof(vSize)); // Occupie sizeof(size_t), write the size as a const char *

    /**
     * Iterate over the vector and write it.
     */
    for (size_t i = 0; i < vSize; i++)
    {
      const std::string &row = this->vector[i];
      size_t rSize = row.size();
      out.write(reinterpret_cast<const char *>(&rSize), sizeof(rSize)); // Occupie sizeof(size_t), write the size as a const char *
      out.write(row.c_str(), rSize);                                    // Occupie size, write the actual string data as const char *
    }

    /**
     * Save the size of the map as the first size_t byte
     * We use this later to read data into the size
     */
    size_t mSize = this->map.size();
    out.write(reinterpret_cast<const char *>(&mSize), sizeof(mSize));

    for (auto &pair : this->map)
    {
      size_t fSize = pair.first.size();
      out.write(reinterpret_cast<const char *>(&fSize), sizeof(fSize));
      out.write(pair.first.c_str(), fSize);

      size_t sSize = pair.second.size();
      out.write(reinterpret_cast<const char *>(&sSize), sizeof(sSize));
      out.write(pair.second.c_str(), sSize);
    }

    out.close();
    return true;
  }

  return false;
}

bool StateSerializer::read()
{
  return this->read(this->saveFile);
}

bool StateSerializer::read(SaveFile saveFile)
{
  std::filesystem::path filepath = std::filesystem::path(saveFile.directory) / (saveFile.name + saveFile.extension);

  std::ifstream in(filepath, std::ios::binary);

  if (in)
  {
    if (!in.is_open())
      return false;

    /**
     * Read the first bytes of size size_t. This is the length of the vector.
     */
    size_t vSize;
    in.read(reinterpret_cast<char *>(&vSize), sizeof(vSize));

    if (!in)
      return false;

    // Resize the vector to fit all elements
    this->vector.resize(vSize);

    /**
     * Iterate over the length of the vector and read it's
     * contents into memory.
     */
    for (size_t i = 0; i < vSize; i++)
    {
      std::string &row = this->vector[i];
      size_t rSize;
      in.read(reinterpret_cast<char *>(&rSize), sizeof(rSize));
      if (!in)
        return false;
      row.resize(rSize);
      in.read(&row[0], rSize);
    }

    /**
     * Read the size of the map as the first size_t byte
     * We use this later to read data into the size
     */
    size_t mSize;
    in.read(reinterpret_cast<char *>(&mSize), sizeof(mSize));

    for (size_t i = 0; i < mSize; i++)
    {
      std::string first;
      size_t fSize;
      in.read(reinterpret_cast<char *>(&fSize), sizeof(fSize));
      if (!in)
        return false;
      first.resize(fSize);
      in.read(&first[0], fSize);

      std::string last;
      size_t sSize;
      in.read(reinterpret_cast<char *>(&sSize), sizeof(sSize));
      if (!in)
        return false;
      last.resize(sSize);
      in.read(&last[0], sSize);

      this->map[first] = last;
    }

    in.close();
    return true;
  }

  return false;
}
