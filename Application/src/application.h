#pragma once

#include <future>
#include <cstdint>
#include <map>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "ECS/registry.h"
#include "ECS/entity.h"
#include "FPS/fps.h"

#include "NativeFileDialog/dialog.h"
#include "Input/input.h"
#include "common.h"

const SDL_WindowFlags APPLICATION_WINDOW_FLAGS = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);

enum class Theme
{
  LIGHT,
  DARK
};

class Application
{
protected:
  SDL_Window *window = nullptr;     // Pointer to the SDL window.
  SDL_Renderer *renderer = nullptr; // Pointer to the SDL renderer used for drawing.
  Registry registry;
  Input input;

private:
  bool isRunning = true;
  Theme theme = Theme::LIGHT;                      // Theme of the app.
  Vec4 backgroundColor = Vec4{255, 255, 255, 255}; // Background color of the window.
  std::map<std::string, ImFont *> fonts;

public:
  Application(SDL_WindowFlags flags = APPLICATION_WINDOW_FLAGS);
  virtual ~Application();

  /**
   * Quit the application gracefully.
   */
  void quit();

  SDL_Window *getWindow()
  {
    return this->window;
  };

  SDL_Renderer *getRenderer()
  {
    return this->renderer;
  };

  Registry *getRegistry()
  {
    return &this->registry;
  };

  Input *getInput()
  {
    return &this->input;
  };

  /**
   * Enable or disable vertical synchronization (vsync).
   *
   * @param vsync An integer flag; if non-zero, vsync is enabled,
   *              otherwise, it is disabled. Vsync helps to prevent
   *              screen tearing by synchronizing the frame rate with
   *              the display's refresh rate.
   */
  void setVSync(int vsync);

  /**
   * Set the dimensions of the SDL window.
   *
   * @param size This new width and height of the viewport
   */
  void setWindowDimensions(Vec2 size);

  /**
   * Returns the window dimensions
   */
  Vec2 getWindowDimensions();

  /**
   * Set the title of the SDL window.
   *
   * @param title A C-style string representing the title of the window.
   */
  void setWindowTitle(const char *title);

  /**
   * Set the background color of the window. Every frame will
   * call SDL_RenderClear with this color to set the background
   *
   * @param backgroundColor a Vec4 that sets that backgound color.
   */
  void setBackgroundColor(Vec4 backgroundColor);

  /**
   * @brief Sets the application's theme.
   *
   * This function updates the current theme of the application.
   *
   * @param theme The Theme
   *
   * @note Make sure to call this function in a context where UI updates
   *       can be safely performed, as it may require re-rendering components.
   */
  void setTheme(Theme theme);

  /**
   * @brief Gets the application's theme.
   *
   * This function returns the current theme of the application.
   */
  Theme getTheme();

  /**
   * @brief Adds fonts that you can use in the application.
   *
   * This function loads a TrueType font (TTF) from the specified file path.
   * The first font that is added will be the default font.
   *
   * @param name The name of the font, you will use this to retrieve the font later.
   *
   * @param ttfPath The file path to the TrueType font (.ttf) file.
   *                This path must be valid and accessible; otherwise,
   *                the font may fail to load.
   * @param fontSize The size of the font to be applied. This parameter
   *                 is optional and defaults to 16.0f if not provided.
   */
  void addFont(std::string name, const char *ttfPath, float fontSize = 16.0f, const ImWchar *glyph_ranges = nullptr);

  /**
   * @param name The name of the font. You set this when adding fonts.
   * @return A pointer to the font
   */
  ImFont *getFont(const char *name);

  /**
   * Open the SDL window.
   */
  void open();

  /**
   * @brief Called to initialize the game or application resources.
   *
   * This method is responsible for setting up necessary components,
   * loading assets, creating game objects, and performing any initial
   * configuration required before the game starts. It should be called
   * at the beginning of the game loop.
   */
  virtual void onInitialize() {};

  /**
   * @brief Called once at the start of each frame.
   *
   * This method is responsible for processing user input events,
   * such as keyboard and mouse actions. It should handle all
   * relevant input before any game state updates occur.
   *
   * @param event The SDL_Event structure to be filled with the next event from the queue, or NULL.
   *
   * @param deltaTime  The time elapsed between the current and the last frame,
   *                   measured in milliseconds. This value can be used for frame-rate independent
   *                   movement and animations.
   */
  virtual void onInput(SDL_Event *event, float deltaTime) {};

  /**
   * @brief Called once after onInput() for each frame.
   *
   * This method is responsible for updating the game/application state.
   * It should incorporate changes based on user input and other
   * game logic, such as physics, animations, and AI updates.
   *
   * @param deltaTime  The time elapsed between the current and the last frame,
   *                   measured in milliseconds. This value can be used for frame-rate independent
   *                   movement and animations.
   */
  virtual void onUpdate(float deltaTime) {};

  /**
   * @brief Called once after onUpdate() for each frame.
   *
   * This method is responsible for rendering the current state of
   * the game/application to the screen. It should clear the
   * previous frame and draw all visual elements based on the
   * updated state.
   *
   * @param deltaTime  The time elapsed between the current and the last frame,
   *                   measured in milliseconds. This value can be used for frame-rate independent
   *                   movement and animations.
   */
  virtual void onDraw(float deltaTime) {};

  /**
   * @brief Called to clean up resources before the game or application exits.
   *
   * This method is responsible for releasing or unloading any resources
   * that were allocated during initialization, such as textures, sounds,
   * and other game data. It should be called before shutting down the
   * application to prevent memory leaks and ensure proper cleanup.
   */
  virtual void onCleanUp() {};

  /**
   * @brief Executes a blocking task on a seperate thread and invokes a callback with its result.
   *
   * This function allows for running a specified task in a separate thread
   * while providing a mechanism to handle the result through a callback function.
   * The task can accept any number of arguments, which will be forwarded
   * appropriately, ensuring both flexibility and type safety.
   *
   * @tparam Callback  The type of the callback function that will be invoked with the result of the task.
   * @tparam Task      The type of the task function. This should be a callable type.
   * @tparam Args      A variadic template parameter representing the types of the arguments passed to the task.
   *
   * @param callback The function to be called with the result of the task. It should match the type Callback.
   * @param task The function to be executed. It should match the type Task.
   * @param args The arguments to be passed to the task function.
   */
  template <typename Callback, typename Task, typename... TaskArgs>
  void AsyncTask(Callback callback, Task task, TaskArgs &&...args)
  {
    std::thread([callback, task, args...]() mutable
                { callback(task(std::forward<TaskArgs>(args)...)); })
        .detach();
  }
};