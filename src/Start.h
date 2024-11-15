#pragma once

#include <string>
#include <vector>

#include "Application.h"

class Project;

class Start : public Application
{
private:
  Project *project;
  unsigned int banner;
  std::vector<std::string> *recentProjects;

public:
  Start(Project *project);
  ~Start() = default;

  void onDraw(float deltaTime) override;
};