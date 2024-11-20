#include "ProjectManager.h"

bool ProjectManager::isRunning()
{
  return this->running;
}

void ProjectManager::setApplication(Application *app)
{
  this->app = app;
}

void ProjectManager::restart(SaveFile saveFile)
{
  this->state.setSaveFile(saveFile);
  this->state.read();
  this->app->quit();
}

void ProjectManager::quit()
{
  this->running = false;
  this->app->quit();
}

void ProjectManager::save()
{
  this->state.write();
}

void ProjectManager::saveAs()
{
  auto callback = [this](std::string path)
  {
    if (path.empty())
      return;
    SaveFile saveFile(path);
    this->state.write(saveFile);
    this->state.setSaveFile(saveFile);
    this->app->setWindowTitle((saveFile.name + " - Sprite Rigger").c_str());
  };

  NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", SAVE_FILE_EXTENSION}};
  this->app->AsyncTask(callback, NativeFileDialog::SaveFile, this->app->getWindow(), filters, 1);
}

void ProjectManager::open()
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

void ProjectManager::openNew()
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