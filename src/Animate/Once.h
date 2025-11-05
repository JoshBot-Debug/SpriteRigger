#pragma once
#include "Animation.h"
#include "System.h"
#include "Timeline.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Animate {

template <typename T>
concept Interpolatable = std::is_arithmetic_v<T> ||
                         requires(T a, T b, float t) { glm::mix(a, b, t); };

template <typename T>
concept Lerpable = requires(T a, T b, float t) {
  { a + (b - a) * t } -> std::same_as<T>;
};

template <Interpolatable T, typename P>
class Once : public ITimeline<Once<T, P>>,
             public Animation,
             public std::enable_shared_from_this<Once<T, P>> {
private:
  struct AnimationValue {
    T *current = nullptr;
    T start{};
    T end{};
  };

private:
  std::shared_ptr<Animate::Timeline> m_Timeline = nullptr;
  std::vector<AnimationValue> m_Values{};

  P *m_UserData = nullptr;
  void (*m_OnUpdate)(P *userData) = nullptr;

public:
  static std::shared_ptr<Once<T, P>>
  Create(std::shared_ptr<Animate::Timeline> timeline =
             Animate::Timeline::Create()) {
    auto ptr = std::make_shared<Once<T, P>>();
    ptr->m_Timeline = timeline;
    return ptr;
  }

  std::shared_ptr<Once<T, P>> Timeline(Animate::Timeline *timeline) {
    m_Timeline = timeline;
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Duration(float duration) override {
    m_Timeline->Duration(duration);
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Elapsed(float elapsed) override {
    m_Timeline->Elapsed(elapsed);
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Play() override {
    m_Timeline->Play();
    System::Instance().Attach(this->shared_from_this());
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Pause() override {
    m_Timeline->Pause();
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Reset() override {
    m_Timeline->Reset();
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> Value(T *current, const T &final) {
    m_Values.emplace_back(current, *current, final);
    return this->shared_from_this();
  }

  std::shared_ptr<Once<T, P>> OnUpdate(P *userData,
                                    void (*onUpdate)(P *userData)) {
    m_UserData = userData;
    m_OnUpdate = onUpdate;
    return this->shared_from_this();
  }

  float Update() {
    float t = m_Timeline->GetTime();
    for (auto &value : m_Values)
      *value.current = Lerp(value.start, value.end, t);
    if (m_OnUpdate)
      m_OnUpdate(m_UserData);
    return t;
  }

  std::shared_ptr<Animate::Timeline> GetTimeline() const override {
    return m_Timeline;
  };

private:
  static T Lerp(const T &a, const T &b, float t) {
    if constexpr (Lerpable<T>)
      return a + (b - a) * t;
    else
      return glm::mix(a, b, t);
  }
};

} // namespace Animate
