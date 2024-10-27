#include "window.h"
#include <stdio.h>

Window::Window() : title("Window"), window(NULL), surface(NULL), h(640), w(480)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    printf("SDL_Init Error: %s\n", SDL_GetError());
}

Window::~Window()
{
  SDL_DestroyWindow(window);
  this->window = NULL;
  this->surface = NULL;
}


void Window::open()
{
  this->window = SDL_CreateWindow(this->title.c_str(), this->w, this->h, SDL_WINDOW_RESIZABLE);
  if (!this->window)
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
}

void Window::setDimensions(int w, int h)
{
  this->w = w;
  this->h = h;
}

void Window::setTitle(const std::string &title)
{
  this->title = title;
}

SDL_Surface *Window::getSurface(bool refresh)
{
  if (refresh || !this->surface)
    this->surface = SDL_GetWindowSurface(window);

  return this->surface;
}

SDL_PixelFormat Window::getSurfaceFormat()
{
  if (!this->surface)
    this->surface = SDL_GetWindowSurface(window);

  return this->surface->format;
}

SDL_Window *Window::getWindow()
{
  return this->window;
}