#include "System.h"

#include "Animation.h"
#include "Timeline.h"

namespace Animate {

void System::Attach(const std::shared_ptr<Animation> &animation) {
  m_Animations.emplace_back(animation);
}

void System::Update(float deltaTime) {
  for (auto &animation : m_Animations)
    animation->GetTimeline()->Update(deltaTime);

  for (auto it = m_Animations.begin(); it != m_Animations.end();) {
    float t = (*it)->Update();
    if (t <= 0.0f || t >= 1.0f)
      it = m_Animations.erase(it);
    else
      ++it;
  }
}

} // namespace Animate