#pragma once

#include <filesystem>

#include "application.h"
#include "StateSerializer/stateSerializer.h"

inline const char *SAVE_FILE_EXTENSION = "sprig";

class Project
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

  Project() = default;
  ~Project() = default;

  bool isRunning()
  {
    return this->running;
  }

  void setApplication(Application *app)
  {
    this->app = app;
  }

  Application *getApplication()
  {
    return this->app;
  }

  void restart(SaveFile saveFile)
  {
    this->state.setSaveFile(saveFile);
    this->state.read();
    this->app->quit();
  }

  void quit()
  {
    this->running = false;
    this->app->quit();
  }

  void save()
  {
    this->state.write();
  }

  void saveAs()
  {
    auto callback = [this](std::string path)
    {
      if (path.empty())
        return;
      SaveFile saveFile(path);
      this->state.write(saveFile);
      this->state.setSaveFile(saveFile);
      this->getApplication()->setWindowTitle((saveFile.name + " - Sprite Rigger").c_str());
    };

    NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", SAVE_FILE_EXTENSION}};
    this->app->AsyncTask(callback, NativeFileDialog::SaveFile, this->app->getWindow(), filters, 1);
  }

  void open()
  {
    auto callback = [this](std::string path)
    {
      if (path.empty())
        return;
      this->restart(SaveFile{path});
    };

    NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", SAVE_FILE_EXTENSION}};
    this->app->AsyncTask(callback, NativeFileDialog::SelectFile, this->app->getWindow(), filters, 1);
  }

  void openNew()
  {
    auto callback = [this](std::string path)
    {
      if (path.empty())
        return;
      SaveFile saveFile(path);
      this->state.write(saveFile);
      this->restart(saveFile);
    };

    NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", SAVE_FILE_EXTENSION}};
    this->app->AsyncTask(callback, NativeFileDialog::SaveFile, this->app->getWindow(), filters, 1);
  }
};