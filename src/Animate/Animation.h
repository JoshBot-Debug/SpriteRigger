#pragma once

#include <memory>

namespace Animate {
class Timeline;

class Animation {
public:
  virtual std::shared_ptr<Animate::Timeline> GetTimeline() const = 0;

  virtual float Update() = 0;
};

} // namespace Animate
