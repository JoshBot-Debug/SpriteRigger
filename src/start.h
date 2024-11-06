#pragma once

#include <string>
#include <filesystem>

#include "utility.h"
#include "application.h"
#include "bannerImage.h"
#include "ProjectManager/projectManager.h"

class Start : public Application
{
private:
  ProjectManager *projectManager;
  SDL_Texture *banner;

public:
  Start(ProjectManager *projectManager) : Application(APPLICATION_WINDOW_FLAGS | SDL_WINDOW_BORDERLESS), projectManager(projectManager)
  {
    SDL_IOStream *stream = SDL_IOFromConstMem(BANNER_IMAGE_BUFFER, BANNER_IMAGE_BUFFER_SIZE);
    SDL_Surface *surface = IMG_Load_IO(stream, true);
    this->banner = SDL_CreateTextureFromSurface(this->renderer, surface);
  };

  ~Start() = default;

  void onDraw(float deltaTime) override;

  void SelectProjectDirectory();
  void CreateNewProject();

  void loadProjectFromDirectory(std::string path);
  void createProjectInDirectory();
};