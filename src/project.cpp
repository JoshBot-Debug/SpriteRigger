#include "project.h"
#include "Serializer/serializer.h"

const char *PROJECT_FILE = "/project.spriterigger";

bool Project::isReady()
{
  if (this->data.directory.empty())
    return false;
  if (this->data.name.empty())
    return false;
  return true;
}

void Project::serialize()
{
  Serializer::out(this->data.directory + PROJECT_FILE, [this](auto &stream)
                  {
    Serializer::Serialize::string(stream, this->data.name);
    Serializer::Serialize::string(stream, this->data.directory); });
}

void Project::deserialize(const std::string &path)
{
  Serializer::in(path + PROJECT_FILE, [this](auto &stream)
                 {
    Serializer::Deserialize::string(stream, this->data.name);
    Serializer::Deserialize::string(stream, this->data.directory); });
}