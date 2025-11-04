#pragma once

#include <memory>
#include <vector>

namespace Animate {

class Animation;

class System {
private:
  System() = default;

  ~System() = default;

  System(const System &) = delete;

  System &operator=(const System &) = delete;

private:
  std::vector<std::shared_ptr<Animation>> m_Animations{};

public:
  static System &Instance() {
    static System instance;
    return instance;
  }

  void Attach(const std::shared_ptr<Animation> &animation);

  void Update(float deltaTime);
};

} // namespace Animate
