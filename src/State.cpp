#include "State.h"
#include "Serializer/Serializer.h"

State::State() : m_Serializer({.magic = "SPRIG", .version = 1}) {}

std::shared_ptr<SerializableLayer>
State::Register(const std::shared_ptr<SerializableLayer> &layer) {
  m_Layers.emplace_back(layer);
  return layer;
}

void State::New() {
  m_Serializer.Write(".sprig");

  m_Serializer.Load("save.sprig");
}

void State::Open() {}

void State::Open(const std::string &filepath) {}

void State::Save() {
  for (auto &layer : m_Layers)
    layer->Save(m_Serializer);

  m_Serializer.Save(".sprig", "save.sprig");
}

void State::SaveAs() {}

void State::Restore(const std::string &filepath) {
  m_Serializer.Load(filepath);

  for (auto &layer : m_Layers)
    layer->Restore(m_Serializer);
}
