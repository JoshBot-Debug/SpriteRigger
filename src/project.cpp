#include "project.h"

const char *PROJECT_FILE = "/project.spriterigger";

bool Project::isReady()
{
  if (this->data.directory.empty())
    return false;
  if (strlen(this->data.name) == 0)
    return false;
  return true;
}

void Project::serialize()
{
  std::ofstream out(this->data.directory + PROJECT_FILE, std::ios::binary);
  if (out)
  {
    // Name
    out.write(this->data.name, sizeof(this->data.name));
  
    // Directory
    size_t directoryLength = this->data.directory.size();
    out.write((const char *)(&directoryLength), sizeof(directoryLength));
    out.write(this->data.directory.c_str(), directoryLength);

    out.close();
  }
  else
    printf("Failed to save the project");
}

void Project::deserialize(const std::string &path)
{
  std::ifstream in(path + PROJECT_FILE, std::ios::binary);
  if (in)
  {
    // Name
    in.read(this->data.name, sizeof(this->data.name));
    this->data.name[127] = '\0';

    // Directory
    size_t directoryLength;
    in.read((char *)(&directoryLength), sizeof(directoryLength));
    this->data.directory.resize(directoryLength);
    in.read(&this->data.directory[0], directoryLength);

    in.close();
  }
  else
    printf("Failed to open the project");
}