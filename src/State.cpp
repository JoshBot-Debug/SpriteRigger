#include "State.h"
#include "Serializer/Serializer.h"

#include "tinyfiledialogs.h"

#include "Utility.h"

inline const std::string RECENT = GetHomeDirectory() + "/.spriterigger/.sprig";

inline const unsigned int MAX_RECENT = 20;

State::State() : m_Serializer({.magic = "SPRIG", .version = 1}) {
  SyncRecentProjects();
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

  std::string filepath = AddFileExtension(file, ".sprig");

  m_Serializer.Write(filepath);

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  SyncRecentProjects(filepath);

  return true;
}

bool State::Open() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_openFileDialog("Select a project", "", 1, filter,
                                           "SpriteRigger files", 0);

  if (!file)
    return false;

  return Open(std::string(file));
}

bool State::Open(const std::string &filepath) {
  if (!m_Serializer.Load(filepath)) {
    SyncRecentProjects(filepath, true);
    return false;
  }

  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);

  m_Serializer.Clear();

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  SyncRecentProjects(filepath);

  return true;
}

void State::Save() {
  for (auto &layer : m_Layers)
    layer->Save(m_Serializer);

  m_Serializer.Write(m_ProjectFile);
}

void State::SaveAs() {
  const char *filter[] = {"*.sprig"};

  const char *file = tinyfd_openFileDialog("Save as", "", 1,
                                           filter, "SpriteRigger files", 0);

  if (!file)
    return;

  std::string filepath = AddFileExtension(file, ".sprig");

  m_Serializer.Write(filepath);

  m_ProjectFile = filepath;

  SyncRecentProjects(filepath);
}

void State::SyncRecentProjects() {
  // Load recent projects from disk
  m_Serializer.Load(RECENT);

  m_RecentProjects.clear();

  // Read all recent projects
  for (auto &bytes : m_Serializer.GetAll("recent"))
    m_RecentProjects.emplace_back(
        std::string(reinterpret_cast<char *>(bytes.data()), bytes.size()));

  // Clear loaded data
  m_Serializer.Clear();
}

void State::SyncRecentProjects(const std::string &filepath, bool remove) {

  SyncRecentProjects();

  if (filepath.size() == 0)
    return;

  // Find the path, erase it if it exists
  auto it =
      std::find(m_RecentProjects.begin(), m_RecentProjects.end(), filepath);
  if (it != m_RecentProjects.end())
    m_RecentProjects.erase(it);

  // Clear all recent projects after max
  if (m_RecentProjects.size() >= MAX_RECENT)
    m_RecentProjects.erase(m_RecentProjects.begin() + MAX_RECENT - 1,
                           m_RecentProjects.end());

  // Push the project to the top
  if (remove == false)
    m_RecentProjects.insert(m_RecentProjects.begin(), filepath);

  // Stage all recent projects
  for (auto &r : m_RecentProjects)
    m_Serializer.Stage("recent", r.c_str(), r.size());

  // Write recent projects to disk
  m_Serializer.Write(RECENT);
}
