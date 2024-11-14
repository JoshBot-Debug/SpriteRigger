#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "Utility.h"
#include "Application.h"
#include "BannerImage.h"

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