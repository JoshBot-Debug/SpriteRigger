#include "Timeline.h"

#include "System.h"

#include <glm/glm.hpp>

namespace Animate {

std::shared_ptr<Timeline> Timeline::Create() {
  return std::make_shared<Timeline>();
}

std::shared_ptr<Timeline> Timeline::Duration(float duration) {
  m_Duration = duration;
  return this->shared_from_this();
}

std::shared_ptr<Timeline> Timeline::Elapsed(float elapsed) {
  m_Elapsed = elapsed;
  return this->shared_from_this();
}

std::shared_ptr<Timeline> Timeline::Play() {
  m_Play = true;
  return this->shared_from_this();
}

std::shared_ptr<Timeline> Timeline::Pause() {
  m_Play = false;
  return this->shared_from_this();
}

std::shared_ptr<Timeline> Timeline::Reset() {
  m_Time = 0.0f;
  m_Elapsed = 0.0f;
  m_Play = false;
  return this->shared_from_this();
}

float Timeline::Update(float deltaTime) {
  m_Elapsed += deltaTime;
  m_Time = glm::clamp(m_Elapsed / m_Duration, 0.0f, 1.0f);

  if (m_Time >= 1.0f)
    m_Play = false;

  return m_Time;
}

float Timeline::GetTime() const { return m_Time; }

} // namespace Animate