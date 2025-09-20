#pragma once

struct Window;

class Layer {
protected:
  Window *m_Window = nullptr;

public:
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}

  virtual void OnUpdate(float deltaTime) {}
  virtual void OnRender() {}

  void SetWindow(Window *window) { m_Window = window; }
};