#include "State.h"
#include "Serializer/Serializer.h"

#include "tinyfiledialogs.h"

#include "Utility.h"

// inline const std::string RECENT =
// GetHomeDirectory() + "/.spriterigger/recent.sprig";

inline const std::string RECENT = "recent.sprig";

State::State() : m_Serializer({.magic = "SPRIG", .version = 1}) {
  m_Serializer.Load(RECENT);
}

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

  m_Serializer.Write(filepath);

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  UpdateRecentProjects(filepath);

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

  UpdateRecentProjects(filepath);

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

  m_Serializer.Write(m_ProjectFile);
}

void State::SaveAs() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_openFileDialog("Create a new project", "", 1,
                                           filter, "SpriteRigger files", 0);

  if (!file)
    return;

  std::string filepath = std::string(file);

  m_Serializer.Write(filepath);

  m_ProjectFile = filepath;

  UpdateRecentProjects(filepath);
}

void State::UpdateRecentProjects(const std::string &filepath) {
  // Add to recent files
  m_Serializer.Stage("recent", Serializer::Type::RAW, filepath.c_str(),
                     filepath.size());
  m_Serializer.Write(RECENT);
}
