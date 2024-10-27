#pragma once

#include <SDL3/SDL.h>
#include "Window/window.h"
#include "Render/render.h"
#include "FrameRate/frame_rate.h"

class Application
{

private:
  void loop();

protected:
  FrameRate frame_rate;
  Window window;
  Render render;

public:
  Application() : frame_rate(), window(), render() {}
  virtual ~Application();

  void setWindowTitle(const std::string &title);
  void setWindowDimensions(int w, int h);

  void launch();

  virtual void onDraw(float deltaTime) = 0;
  virtual void onInput(SDL_Event event) = 0;
};