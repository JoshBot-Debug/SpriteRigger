#pragma once
#include <SDL3/SDL.h>
#include <string>

class Window
{
private:
  std::string title;
  int w;
  int h;
  SDL_Window *window;
  SDL_Surface *surface;

public:
  Window();
  ~Window();

  void open();
  void setTitle(const std::string &title);
  void setDimensions(int w, int h);
  
  SDL_Window *getWindow();
  SDL_Surface *getSurface(bool refresh = false);
  SDL_PixelFormat getSurfaceFormat();
};