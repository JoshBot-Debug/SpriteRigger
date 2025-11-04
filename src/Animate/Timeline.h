#pragma once

#include <memory>
#include <string>

namespace Animate {

template <typename T> class ITimeline {
public:
  virtual ~ITimeline() = default;

  virtual std::shared_ptr<T> Duration(float duration) = 0;

  virtual std::shared_ptr<T> Elapsed(float elapsed) = 0;

  virtual std::shared_ptr<T> Play() = 0;

  virtual std::shared_ptr<T> Pause() = 0;

  virtual std::shared_ptr<T> Reset() = 0;

  virtual float Update(float deltaTime) { return 0.0f; };
};

class Timeline : public ITimeline<Timeline>,
                 public std::enable_shared_from_this<Timeline> {
private:
  float m_Time = 0.0f;
  float m_Duration = 1.0f;
  float m_Elapsed = 0.0f;
  bool m_Play = false;

public:
  static std::shared_ptr<Timeline> Create();

  std::shared_ptr<Timeline> Duration(float duration) override;

  std::shared_ptr<Timeline> Elapsed(float elapsed) override;

  std::shared_ptr<Timeline> Play() override;

  std::shared_ptr<Timeline> Pause() override;

  std::shared_ptr<Timeline> Reset() override;

  float Update(float deltaTime) override;

  float GetTime() const;
};

} // namespace Animate
