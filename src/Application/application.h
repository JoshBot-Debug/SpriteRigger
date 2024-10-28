#pragma once

#include <SDL3/SDL.h>
#include "Window/window.h"
#include "Render/render.h"
#include "FrameRate/frame_rate.h"
#include "MouseEventHandler/mouse_event_handler.h"

class Application
{

private:
  void loop();

protected:
  Window window;
  Render render;
  FrameRate frame_rate;
  MouseEventHandler mouse_event_handler;

public:
  Application();
  virtual ~Application();

  void setWindowTitle(const std::string &title);
  void setWindowDimensions(int w, int h);

  void launch(SDL_Color backgroundColor = {255, 255, 255, SDL_ALPHA_OPAQUE});

  virtual void onDraw(float deltaTime) = 0;
  virtual void onInput(SDL_Event event) = 0;
};