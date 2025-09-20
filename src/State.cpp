#include "State.h"
#include "Serializer/Serializer.h"

#include "tinyfiledialogs.h"

#include "Utility.h"

State::State() : m_Serializer({.magic = "SPRIG", .version = 1}) {}

std::shared_ptr<SerializableLayer>
State::Register(const std::shared_ptr<SerializableLayer> &layer) {
  m_Layers.emplace_back(layer);
  return layer;
}

bool State::New() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_saveFileDialog(
      "Create a new project", "project.sprig", 1, filter, "SpriteRigger files");

  if (!file)
    return false;

  std::string filepath = std::string(file);

  m_Serializer.Write(filepath + ".tmp");
  m_Serializer.Save(filepath + ".tmp", filepath);

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  return true;
}

bool State::Open() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_openFileDialog("Select a project", "", 1, filter,
                                           "SpriteRigger files", 0);

  if (!file)
    return false;

  std::string filepath = std::string(file);

  m_Serializer.Load(filepath);

  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  return true;
}

void State::Open(const std::string &filepath) {
  m_Serializer.Load(filepath);

  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);

  m_ProjectFile = filepath;

  m_IsInitialized = true;
}

void State::Save() {
  for (auto &layer : m_Layers)
    layer->Save(m_Serializer);

  m_Serializer.Write(m_ProjectFile + ".tmp");
  m_Serializer.Save(m_ProjectFile + ".tmp", m_ProjectFile);
}

void State::SaveAs() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_openFileDialog("Create a new project", "", 1,
                                           filter, "SpriteRigger files", 0);

  if (!file)
    return;

  std::string filepath = std::string(file);

  m_Serializer.Write(filepath + ".tmp");

  m_Serializer.Save(filepath + ".tmp", filepath);

  m_ProjectFile = filepath;
}