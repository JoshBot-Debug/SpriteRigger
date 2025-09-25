#pragma once

#include "imgui.h"

#include "Window/Window.h"

#include "SerializableLayer.h"
#include "Shader/Shader.h"
#include "State.h"

#include "Application/Components.h"
#include <vector>

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;

  Shader m_Shader;

  GLuint boneVAO = 0;
  GLuint boneInstanceVBO = 0;

  GLuint fbo = 0;
  GLuint colorTex = 0;
  GLuint depthRBO = 0;

  std::vector<CBone> m_Bones;

public:
  explicit ViewportLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnUpdate(float deltaTime) override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;
};
