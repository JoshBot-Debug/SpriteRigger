#pragma once

#include <functional>

#include "SDL3/SDL.h"

#include "imgui.h"

class Viewport
{
private:
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

  float h, w;                                                             // Height and width of the viewport
  SDL_Color backgroundColor = SDL_Color{255, 255, 255, SDL_ALPHA_OPAQUE}; // Background color

  // Frees the texture and any other resources
  void free();

  // Resizes the viewport and updates the texture accordingly
  void resize(float w, float h);

public:
  // Default constructor
  Viewport() = default;

  // Constructor that initializes the viewport with a renderer and dimensions
  Viewport(SDL_Renderer *renderer, int w, int h, SDL_Color backgroundColor);

  // Destructor to clean up resources
  ~Viewport();

  // Sets the SDL_Renderer for the viewport
  // Note: This should be called last, before invoking the draw function and only once.
  void setRenderer(SDL_Renderer *renderer);

  // Sets the dimensions of the viewport
  void setDimensions(float w, float h);

  // Sets the background color of the viewport
  void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  // Draws the viewport, executing the provided callback function for custom rendering
  void draw(const char *title, std::function<void()> callback);
};