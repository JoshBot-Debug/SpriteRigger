#pragma once

#include <functional>

#include "SDL3/SDL.h"

#include "imgui.h"

#include "common.h"

class Viewport
{
private:
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

  Vec4 backgroundColor = Vec4{255, 255, 255, 255};

  // Viewport width and height
  Vec2 size{0, 0};

  // Viewport position
  Vec2 position{0, 0};

  /**
   * Frees the texture and any other resources.
   */
  void free();

  /**
   * Resizes the viewport and updates the texture accordingly.
   *
   * @param size This new width and height of the viewport
   */
  void resize(Vec2 size);

public:
  /**
   * Default constructor.
   */
  Viewport() = default;

  /**
   * Constructor that initializes the viewport with a renderer and dimensions.
   *
   * @param renderer The SDL_Renderer used for rendering.
   * @param w The width of the viewport.
   * @param h The height of the viewport.
   * @param backgroundColor The background color of the viewport.
   */
  Viewport(SDL_Renderer *renderer, Vec2 size, Vec4 backgroundColor);

  /**
   * Destructor to clean up resources.
   */
  ~Viewport();

  /**
   * Sets the SDL_Renderer for the viewport.
   *
   * Note: This should be called last, before invoking the draw function and only once.
   *
   * @param renderer The SDL_Renderer to be set for the viewport.
   */
  void setRenderer(SDL_Renderer *renderer);

  /**
   * Sets the dimensions of the viewport.
   *
   * NOTE: This will not update a texture that has already been created
   *       for that, you have to call resize.
   *
   * @param size This new width and height of the viewport
   */
  void setDimensions(Vec2 size);

  /**
   * Sets the background color of the viewport.
   *
   * @param backgroundColor a Vec4 that sets that backgound color.
   */
  void setBackgroundColor(Vec4 backgroundColor);

  /**
   * Draws the viewport, executing the provided callback function for custom rendering.
   *
   * Any rendering done in the callback will be performed inside the viewport that was created.
   *
   * @param title The title to be displayed.
   * @param callback The function to be called for custom rendering.
   */
  void draw(const char *title, std::function<void()> callback);

  /**
   * Takes in the global mouse position and returns its position relative to this viewport.
   *
   * @param gPosition The global mouse position.
   * @return The position of the mouse relative to the viewport.
   */
  Vec2 getMousePosition(Vec2 position);
};