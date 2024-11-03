#pragma once

#include <functional>

#include "SDL3/SDL.h"

#include "imgui.h"

#include "Vec/vec.h"

class Viewport
{
private:
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

  SDL_Color backgroundColor = SDL_Color{255, 255, 255, SDL_ALPHA_OPAQUE}; // Background color

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
   * @param w The new width of the viewport.
   * @param h The new height of the viewport.
   */
  void resize(float w, float h);

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
  Viewport(SDL_Renderer *renderer, float w, float h, SDL_Color backgroundColor);

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
   * @param w The new width of the viewport.
   * @param h The new height of the viewport.
   */
  void setDimensions(float w, float h);

  /**
   * Sets the background color of the viewport.
   *
   * @param r The red component of the background color.
   * @param g The green component of the background color.
   * @param b The blue component of the background color.
   * @param a The alpha (transparency) component of the background color.
   *          Defaults to SDL_ALPHA_OPAQUE
   */
  void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = SDL_ALPHA_OPAQUE);

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