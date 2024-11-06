#include "projectManager.h"
#include "Serializer/serializer.h"

bool ProjectManager::isReady()
{
  if (this->data.directory.empty())
    return false;
  if (this->data.name.empty())
    return false;
  return true;
}

void ProjectManager::serialize()
{
  std::filesystem::path fullPath = std::filesystem::path(this->data.directory) / this->projectFileName;

  Serializer::out(fullPath, [this](auto &stream)
                  {
    Serializer::Serialize::string(stream, this->data.name);
    Serializer::Serialize::string(stream, this->data.directory); });
}

void ProjectManager::deserialize(const std::string &directory)
{
  std::filesystem::path fullPath = std::filesystem::path(directory) / this->projectFileName;

  Serializer::in(fullPath, [this](auto &stream)
                 {
    Serializer::Deserialize::string(stream, this->data.name);
    Serializer::Deserialize::string(stream, this->data.directory); });
}

const char *ProjectManager::getProjectFileName()
{
  return this->projectFileName.c_str();
}
