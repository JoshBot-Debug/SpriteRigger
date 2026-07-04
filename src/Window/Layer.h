#pragma once

struct Window;

class Layer
{
protected:
  Window* m_Window = nullptr;

public:
  virtual ~Layer() = default;

  virtual void OnAttach()
  {
  }

  virtual void OnDetach()
  {
  }

  virtual void OnBegin()
  {
  }

  virtual void OnUpdate(float deltaTime)
  {
  }

  virtual void OnRender()
  {
  }

  virtual void OnEnd()
  {
  }

  void SetWindow(Window* window)
  {
    m_Window = window;
  }
};