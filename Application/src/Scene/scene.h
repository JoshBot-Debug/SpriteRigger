#pragma once

#include <application.h>

class Scene
{
public:
  Application *application;
  Scene(Application *application) : application(application) {};
  virtual ~Scene() = default;

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
};