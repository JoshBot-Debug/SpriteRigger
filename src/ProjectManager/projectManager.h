#pragma once
#include <string>
#include <filesystem>
#include <vector>

#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>

struct RecentProject
{
  std::string id;
  std::string name;
  std::string filepath;

  RecentProject() = default;
  RecentProject(std::string id, std::string name, std::string filepath) : id(id), name(name), filepath(filepath) {}
};

struct ProjectData
{
  std::string id;
  std::string name;
  std::string filepath;
};

class ProjectManager
{
public:
  bool isRunning = true;

  ProjectData data;
  std::string fileExtension;

  int maxRecentProjects;
  std::string recentFileName;
  std::vector<RecentProject> recentProjects;

  ProjectManager(std::string fileExtension, std::string recentFileName = "recent", int maxRecentProjects = 5);

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
   *
   * @param generateUID If set to true, a new UID will be generated for the save file.
   *                    UIDs keep uniqueness even if project names are the same.
   */
  void serialize(bool generateUID = false);

  /**
   * Deserialize the project and load it into memory
   */
  void deserialize(const std::string &filepath);

  /**
   * Returns the project's file extension
   */
  const char *getFileExtension();

  /**
   * Generates a random UID, which is unique for all intensive purposes
   */
  std::string generateUID();

  void updateRecentProjects();

  void quit();
};
