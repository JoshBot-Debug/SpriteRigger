#pragma once

#include <string>
#include "application.h"

class Start : public Application
{
private:
  std::string projectDirectory;
  SDL_Texture *banner = nullptr;

public:
  Start() : Application(APPLICATION_WINDOW_FLAGS | SDL_WINDOW_BORDERLESS)
  {
    SDL_IOStream *stream = SDL_IOFromFile("assets/images/start-project-banner.png", "rb");
    SDL_Surface *surface = IMG_Load_IO(stream, true);
    this->banner = SDL_CreateTextureFromSurface(this->renderer, surface);
  };
  ~Start() = default;

  void onDraw(float deltaTime) override;

  std::string getProjectDirectory();
  void selectProjectDirectory();
};