#pragma once

#include <memory>
#include <vector>

namespace Animate
{

class Animation;

class System
{
private:
  std::vector<std::shared_ptr<Animation>> m_Animations{};

public:
  System()  = default;
  ~System() = default;

  System(const System&)            = delete;
  System& operator=(const System&) = delete;

  template <typename... Args> void Attach(Args&&... animations)
  {
    (m_Animations.emplace_back(std::forward<Args>(animations)), ...);
  }

  void Update(float deltaTime);
};

} // namespace Animate
