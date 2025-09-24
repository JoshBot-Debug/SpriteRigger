#pragma once

#include "Layers/SerializableLayer.h"
#include "Serializer/Serializer.h"

#include <memory>
#include <string>
#include <vector>

class State {
private:
  Serializer m_Serializer;
  Serializer m_RecentSerializer;

  bool m_IsInitialized = false;
  bool m_ApplicationRunning = true;

  std::string m_ProjectFile;

  std::vector<std::string> m_RecentProjects = {};

  std::vector<std::shared_ptr<SerializableLayer>> m_Layers = {};

public:
  State();

  std::shared_ptr<SerializableLayer> Register(const std::shared_ptr<SerializableLayer> &layer);

  bool New();

  bool Open();

  bool Open(const std::string &filepath);

  void Save();

  void SaveAs();

  void SyncRecentProjects();

  void SyncRecentProjects(const std::string &filepath, bool remove = false);

  bool IsApplicationRunning() { return m_ApplicationRunning; }

  bool IsInitialized() { return m_IsInitialized; }

  void Quit() { m_ApplicationRunning = false; }

  std::vector<std::string> GetRecentProjects() { return m_RecentProjects; };
};