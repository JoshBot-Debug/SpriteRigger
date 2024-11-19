#pragma once

#include <filesystem>

#include "Application.h"
#include "StateSerializer/StateSerializer.h"

inline const char *SAVE_FILE_EXTENSION = "sprig";

class ProjectManager
{
private:
  Application *app;

  /**
   * Controls if the project is running or not.
   * If this is true, everytime the app quits,
   * it will restart until running is false.
   */
  bool running = true;

public:
  /**
   * The state of the project
   * Contains the active project, preferences, settings, etc,
   * everything specific to this project. It also contains
   * all the saved data.
   */
  StateSerializer state;

  /**
   * The recent projects.
   */
  StateSerializer recent;

  ProjectManager() = default;
  ~ProjectManager() = default;

  /**
   * Is the application project still running.
   * Used to identify if the app should reastart if
   * it was closed.
   */
  bool isRunning();

  /**
   * Set the application.
   */
  void setApplication(Application *app);

  /**
   * Used to restart the app with the selected save file.
   */
  void restart(SaveFile saveFile);

  /**
   * Quit the app and the project.
   * This function will close the app
   */
  void quit();

  void save();

  void saveAs();

  void open();

  void openNew();
};