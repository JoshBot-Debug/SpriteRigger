#include "State.h"
#include "Serializer/Serializer.h"

#include "tinyfiledialogs.h"

#include "Utility.h"

// inline const std::string RECENT =
// GetHomeDirectory() + "/.spriterigger/recent.sprig";

inline const std::string RECENT = "recent.sprig";

inline const unsigned int MAX_RECENT = 20;

State::State() : m_Serializer({.magic = "SPRIG", .version = 1}) {
  m_Serializer.Load(RECENT);

  auto buffer = m_Serializer.GetAll("recent");

  m_Serializer.Clear();

  for (auto &b : buffer)
    m_RecentProjects.emplace_back(
        std::string(reinterpret_cast<char *>(b.data()), b.size()));
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

  Open(std::string(file));

  return true;
}

void State::Open(const std::string &filepath) {
  m_Serializer.Load(filepath);

  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);

  m_Serializer.Clear();

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  UpdateRecentProjects(filepath);
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

  m_Serializer.Load(RECENT);

  auto buffer = m_Serializer.GetAll("recent");

  m_Serializer.Clear();

  std::vector<std::string> recent = {};

  for (auto &b : buffer)
    recent.emplace_back(
        std::string(reinterpret_cast<char *>(b.data()), b.size()));

  auto it = std::find(recent.begin(), recent.end(), filepath);
  if (it != recent.end())
    recent.erase(it);

  if (recent.size() >= MAX_RECENT)
    recent.erase(recent.begin() + MAX_RECENT - 1, recent.end());

  recent.insert(recent.begin(), filepath);

  // Add to recent files
  for (auto &r : recent)
    m_Serializer.Stage("recent", r.c_str(), r.size());

  m_Serializer.Write(RECENT);
}
