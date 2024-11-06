#pragma once

#pragma once
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

struct ProjectData
{
  char name[128];
  std::string directory;
};

class Project
{
private:
public:
  ProjectData data;

  /**
   * Checks if the project has everything required to initialize
   * @return boolean
   */
  bool isReady();

  /**
   * Returns a pointer to ProjectData that can be mutated.
   */
  ProjectData *getData()
  {
    return &this->data;
  }

  /**
   * Serialize the project and save it to disk
   */
  void serialize();

  /**
   * Deserialize the project and load it into memory
   */
  void deserialize(const std::string &path);
};
