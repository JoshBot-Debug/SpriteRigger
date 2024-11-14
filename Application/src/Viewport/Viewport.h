#pragma once

#include <functional>

#include "Application.h"
#include "Scene/Scene.h"

class Viewport : public Scene
{
private:
  Application *application;

  unsigned int texture;
  unsigned int framebuffer;

  const char *title = "Viewport";

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

  // Viewport clear color
  glm::vec4 backgroundColor = glm::vec4{0, 0, 0, 1.0f};

  // Viewport width and height
  glm::vec2 size{1080, 720};

  // Viewport position
  glm::vec2 position{0, 0};

  /**
   * Resizes the viewport and updates the texture accordingly.
   *
   * @param size This new width and height of the viewport
   */
  void resize(glm::vec2 size);

  /**
   * Create the framebuffer and the texture to render to.
   */
  void createFrameBuffer();

public:
  Viewport(Application *application);

  virtual ~Viewport();

  /**
   * @brief Sets the title of the window.
   *
   * This method updates the window title to the specified string.
   *
   * @param title A pointer to a null-terminated character string that
   *              represents the new title for the window.
   *              It should not be modified after this call.
   */
  void setTitle(const char *title);

  /**
   * @brief Sets the flags for the window.
   *
   * This method configures the window's behavior and appearance
   * using the specified ImGuiWindowFlags.
   *
   * @param flags A bitwise combination of ImGuiWindowFlags that determine
   *              the properties of the window, such as whether it is
   *              collapsible, resizable, or has a title bar.
   */
  void setWindowFlags(ImGuiWindowFlags flags);

  /**
   * Sets the dimensions of the viewport.
   *
   * NOTE: This will not update a texture that has already been created
   *       for that, you have to call resize.
   *
   * @param size This new width and height of the viewport
   */
  void setDimensions(glm::vec2 size);

  /**
   * Sets the background color of the viewport.
   *
   * @param backgroundColor a Vec4 that sets that backgound color.
   */
  void setBackgroundColor(glm::vec4 backgroundColor);

  /**
   * Do not override this method, the implimentation is done in the viewport.
   * Instead use onDrawViewport()
   */
  void onDraw(float deltaTime) final;

  /**
   * Use this in place of onDraw();
   */
  virtual void onDrawViewport(float deltaTime) = 0;

  /**
   * Get the size of the viewport
   */
  glm::vec2 getSize();

  /**
   * Get the position of the viewport
   */
  glm::vec2 getPosition();
};