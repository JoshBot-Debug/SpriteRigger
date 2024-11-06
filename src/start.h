#pragma once

#include <string>
#include "application.h"
#include "bannerImage.h"
#include "spriteRigger.h"

class Start : public Application
{
private:
  Project project;
  SDL_Texture *banner = nullptr;

public:
  Start() : Application(APPLICATION_WINDOW_FLAGS | SDL_WINDOW_BORDERLESS)
  {
    SDL_IOStream *stream = SDL_IOFromConstMem(BANNER_IMAGE_BUFFER, BANNER_IMAGE_BUFFER_SIZE);
    SDL_Surface *surface = IMG_Load_IO(stream, true);
    this->banner = SDL_CreateTextureFromSurface(this->renderer, surface);
  };
  ~Start() = default;

  void onDraw(float deltaTime) override;

  Project getProject();

  void SelectProjectDirectory();
  void CreateNewProject();

  void loadProjectFromDirectory(std::string path);
  void createProjectInDirectory();
};