#pragma once

#include <string>
#include <vector>

#include "Application.h"
#include "Manager/ProjectManager/ProjectManager.h"

class Start : public Application
{
private:
  ProjectManager *projectManager;
  unsigned int banner;
  std::vector<std::string> *recentProjects;

public:
  Start(ProjectManager *projectManager);
  ~Start() = default;

  void onDraw(float deltaTime) override;
};