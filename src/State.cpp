#include "State.h"
#include "Serializer/Serializer.h"

#include "tinyfiledialogs.h"

#include "Utility.h"

#include "Application/Components.h"
#include "ECS2/Registry.h"
#include "ServiceLocator/ServiceLocator.h"

#include <cstring>

inline const std::string RECENT = GetHomeDirectory() + "/.spriterigger/.sprig";

inline const unsigned int MAX_RECENT = 20;

inline const Serializer::Options serializerOptions = {.magic = "SPRIG",
                                                      .version = 1};

State::State()
    : m_Serializer(serializerOptions), m_RecentSerializer(serializerOptions) {
  SyncRecentProjects();
}

std::shared_ptr<SerializableLayer>
State::Register(const std::shared_ptr<SerializableLayer> &layer) {
  auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [&layer](auto &i) {
    return typeid(*layer) == typeid(*i);
  });

  if (it != m_Layers.end())
    m_Layers.erase(it);

  return m_Layers.emplace_back(layer);
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

  SyncRecentProjects(filepath);

  m_ProjectFile = filepath;

  m_IsInitialized = true;

  return true;
}

void State::Restore() {
  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);

  // Load the ECS state
  const auto &registry = ServiceLocator::Get<ECS2::Registry>();

  registry->Remove();

  std::vector<std::vector<uint8_t>> entities =
      m_Serializer.GetAll("entity:bone");

  // TODO, do not use entity id as the id for hierarchy.
  for (auto &buffer : entities) {
    size_t offset = 0;

    uint8_t *ptr = buffer.data();

    uint32_t id;
    std::memcpy(&id, ptr, sizeof(id));
    ptr += sizeof(id);

    ECS2::Entity *entity = registry->CreateEntity<EBone>();

    CBone *bone = entity->Add<CBone>();
    CHierarchy *hierarchy = entity->Add<CHierarchy>();
    CFlags *flags = entity->Add<CFlags>();

    std::memcpy(bone, ptr, sizeof(CBone));
    ptr += sizeof(CBone);

    std::memcpy(hierarchy, ptr, sizeof(CHierarchy));
    ptr += sizeof(CHierarchy);

    std::memcpy(flags, ptr, sizeof(CFlags));
  }

  m_Serializer.Clear();
}

void State::Save() {
  m_Serializer.Clear();

  for (auto &layer : m_Layers)
    layer->Save(m_Serializer);

  // Save the ECS state
  const auto &registry = ServiceLocator::Get<ECS2::Registry>();

  auto serialize = [](std::vector<uint8_t> &buffer, const void *data,
                      uint32_t size) {
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(data);
    buffer.insert(buffer.end(), ptr, ptr + size);
  };

  // Serialize bone entity
  for (auto &entity : registry->GetEntities<EBone>()) {

    const auto &[bone, hierarchy, flags] =
        entity->Collect<CBone, CHierarchy, CFlags>();

    std::vector<uint8_t> buffer;
    uint32_t id = entity->GetId();

    serialize(buffer, &id, sizeof(uint32_t));
    serialize(buffer, bone, sizeof(CBone));
    serialize(buffer, hierarchy, sizeof(CHierarchy));
    serialize(buffer, flags, sizeof(CFlags));

    m_Serializer.Stage("entity:bone", buffer.data(), buffer.size());
  }

  m_Serializer.Write(m_ProjectFile);
}

void State::SaveAs() {
  const char *filter[] = {"*.sprig"};

  const char *file =
      tinyfd_openFileDialog("Save as", "", 1, filter, "SpriteRigger files", 0);

  if (!file)
    return;

  std::string filepath = AddFileExtension(file, ".sprig");

  SyncRecentProjects(filepath);

  m_ProjectFile = filepath;

  Save();
}

void State::SyncRecentProjects() {
  // Load recent projects from disk
  m_RecentSerializer.Load(RECENT);

  m_RecentProjects.clear();

  // Read all recent projects
  for (auto &bytes : m_RecentSerializer.GetAll("recent"))
    m_RecentProjects.emplace_back(
        std::string(reinterpret_cast<char *>(bytes.data()), bytes.size()));

  // Clear loaded data
  m_RecentSerializer.Clear();
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

  // Clear any previous data
  m_RecentSerializer.Clear();

  // Stage all recent projects
  for (auto &r : m_RecentProjects)
    m_RecentSerializer.Stage("recent", r.c_str(), r.size());

  // Write recent projects to disk
  m_RecentSerializer.Write(RECENT);
}
