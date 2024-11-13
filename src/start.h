#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "utility.h"
#include "application.h"
#include "bannerImage.h"

class Project;

class Start : public Application
{
private:
  Project *project;
  GLuint banner;
  std::vector<std::string> *recentProjects;

public:
  Start(Project *project);
  ~Start() = default;

  void onDraw(float deltaTime) override;
};