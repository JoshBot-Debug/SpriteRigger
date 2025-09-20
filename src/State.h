#pragma once

#include "Layers/SerializableLayer.h"
#include "Serializer/Serializer.h"

#include <memory>
#include <string>
#include <vector>

class State {
private:
  Serializer m_Serializer;

  bool m_IsInitialized = false;
  bool m_StatelicationRunning = true;

  std::string m_ProjectFile;

  std::vector<std::shared_ptr<SerializableLayer>> m_Layers;

public:
  State();

  std::shared_ptr<SerializableLayer>
  Register(const std::shared_ptr<SerializableLayer> &layer);

  bool New();

  bool Open();
  
  void Open(const std::string &filepath);

  void Save();

  void SaveAs();

  void UpdateRecentProjects(const std::string &filepath);

  bool IsApplicationRunning() { return m_StatelicationRunning; }

  bool IsInitialized() { return m_IsInitialized; }

  void Quit() { m_StatelicationRunning = false; }
};