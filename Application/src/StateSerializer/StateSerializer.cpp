#include "StateSerializer.h"

#include <filesystem>
#include <iostream>
#include <fstream>

SaveFile::SaveFile(std::string filePath)
{
  std::filesystem::path path = filePath;
  this->name = path.stem().string();
  this->extension = path.extension().string();
  this->directory = path.parent_path().string();
}

void SaveFile::set(std::string name, std::string extension, std::string directory)
{
  this->name = name;
  this->extension = extension;
  this->directory = directory;
}

void SaveFile::set(std::string filePath)
{
  std::filesystem::path path = filePath;
  this->name = path.stem().string();
  this->extension = path.extension().string();
  this->directory = path.parent_path().string();
}

SaveFile::operator std::string() const
{
  return std::filesystem::path(this->directory) / (this->name + this->extension);
}

StateSerializer::StateSerializer(SaveFile saveFile) : saveFile(saveFile) {}

void StateSerializer::setSaveFile(SaveFile saveFile)
{
  this->saveFile = saveFile;
}

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

SaveFile *StateSerializer::getSaveFile()
{
  return &this->saveFile;
}

std::string *StateSerializer::getSaveFileName()
{
  return &this->saveFile.name;
}

std::string *StateSerializer::getSaveFileExtension()
{
  return &this->saveFile.extension;
}

std::string *StateSerializer::getSaveFileDirectory()
{
  return &this->saveFile.directory;
}

std::map<std::string, std::string> *StateSerializer::map(const char *key)
{
  return &this->mMap[key];
}

std::vector<std::string> *StateSerializer::vector(const char *key)
{
  return &this->mVec[key];
}

bool StateSerializer::write()
{
  return this->write(this->saveFile);
}

bool StateSerializer::write(std::string filePath)
{
  SaveFile saveFile(filePath);
  return this->write(saveFile);
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
     * mVec
     * Write the size of the map
     */
    size_t mvSize = this->mVec.size();
    out.write(reinterpret_cast<const char *>(&mvSize), sizeof(mvSize));

    for (auto &pair : this->mVec)
    {
      /**
       * Write the key of the map
       */
      size_t fmvSize = pair.first.size();
      out.write(reinterpret_cast<const char *>(&fmvSize), sizeof(fmvSize));
      out.write(pair.first.c_str(), fmvSize);

      /**
       * Write the size of the vector
       */
      size_t smvSize = pair.second.size();
      out.write(reinterpret_cast<const char *>(&smvSize), sizeof(smvSize));

      for (size_t i = 0; i < smvSize; i++)
      {
        /**
         * Write the value of the vector.
         */
        const std::string &row = pair.second[i];
        size_t smvrSize = row.size();
        out.write(reinterpret_cast<const char *>(&smvrSize), sizeof(smvrSize));
        out.write(row.c_str(), smvrSize);
      }
    }

    /**
     * Write the size of the map
     */
    size_t mmSize = this->mMap.size();
    out.write(reinterpret_cast<const char *>(&mmSize), sizeof(mmSize));

    for (auto &pair : this->mMap)
    {
      /**
       * Write the key of the map
       */
      size_t fmmSize = pair.first.size();
      out.write(reinterpret_cast<const char *>(&fmmSize), sizeof(fmmSize));
      out.write(pair.first.c_str(), fmmSize);

      /**
       * Write the size of the value
       */
      size_t smmSize = pair.second.size();
      out.write(reinterpret_cast<const char *>(&smmSize), sizeof(smmSize));

      for (auto &spair : pair.second)
      {
        /**
         * Write the key of the value
         */
        size_t sfmmSize = spair.first.size();
        out.write(reinterpret_cast<const char *>(&sfmmSize), sizeof(sfmmSize));
        out.write(spair.first.c_str(), sfmmSize);

        /**
         * Write the value of the value
         */
        size_t ssmmSize = spair.second.size();
        out.write(reinterpret_cast<const char *>(&ssmmSize), sizeof(ssmmSize));
        out.write(spair.second.c_str(), ssmmSize);
      }
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

bool StateSerializer::read(std::string filePath)
{
  SaveFile saveFile(filePath);
  return this->read(saveFile);
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
     * mVec
     * Read the size of the map
     */
    size_t mvSize;
    in.read(reinterpret_cast<char *>(&mvSize), sizeof(mvSize));
    if (!in)
      return false;

    for (size_t i = 0; i < mvSize; i++)
    {
      /**
       * Read the size of key of the map
       */
      std::string key;
      size_t fmvSize;
      in.read(reinterpret_cast<char *>(&fmvSize), sizeof(fmvSize));
      key.resize(fmvSize);
      in.read(&key[0], fmvSize);

      /**
       * Read the size of the vector
       */
      size_t smvSize;
      in.read(reinterpret_cast<char *>(&smvSize), sizeof(smvSize));

      std::vector<std::string> value;
      value.resize(smvSize);

      for (size_t j = 0; j < smvSize; j++)
      {
        /**
         * Read the value of the vector
         */
        std::string &row = value[j];
        size_t smvrSize;
        in.read(reinterpret_cast<char *>(&smvrSize), sizeof(smvrSize));
        value[j].resize(smvrSize);
        in.read(&row[0], smvrSize);
      }

      mVec[key] = value;
    }

    /**
     * Read the size of the map
     */
    size_t mmSize;
    in.read(reinterpret_cast<char *>(&mmSize), sizeof(mmSize));

    for (size_t i = 0; i < mmSize; i++)
    {

      /**
       * Read the key of the map
       */
      std::string key;
      size_t fmmSize;
      in.read(reinterpret_cast<char *>(&fmmSize), sizeof(fmmSize));
      key.resize(fmmSize);
      in.read(&key[0], fmmSize);

      /**
       * Read the size of the value
       */
      size_t smmSize;
      in.read(reinterpret_cast<char *>(&smmSize), sizeof(smmSize));

      std::map<std::string, std::string> value;

      for (size_t j = 0; j < smmSize; j++)
      {
        /**
         * Write the key of the value
         */
        std::string vKey;
        size_t sfmmSize;
        in.read(reinterpret_cast<char *>(&sfmmSize), sizeof(sfmmSize));
        vKey.resize(sfmmSize);
        in.read(&vKey[0], sfmmSize);

        /**
         * Write the value of the value
         */
        std::string vVal;
        size_t ssmmSize;
        in.read(reinterpret_cast<char *>(&ssmmSize), sizeof(ssmmSize));
        vVal.resize(ssmmSize);
        in.read(&vVal[0], ssmmSize);

        value[vKey] = vVal;
      }

      this->mMap[key] = value;
    }

    in.close();
    return true;
  }

  return false;
}