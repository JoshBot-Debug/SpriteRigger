#include "ViewportLayer.h"

#include "Application/Components.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

const std::string EXE_DIRECTORY = GetExecutableDirectory();

ViewportLayer::ViewportLayer(State *state)
    : m_State(state), m_Grid(&m_Camera) {}

void ViewportLayer::OnAttach() {
  m_Registry = ServiceLocator::Get<Registry>();

  m_Shader.create({
      .name = "default",
      .vertex = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.vert").c_str(),
      .fragment = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.frag").c_str(),
  });

  // Create and bind the VAO
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  float quadVerts[8] = {0.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f};

  GLuint quadVBO;
  glGenBuffers(1, &quadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

  // layout(location = 0) in vec2 a_corner;
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glVertexAttribDivisor(0, 0);

  // Now create the per-instance buffer (CBone instances)
  glGenBuffers(1, &m_InstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  // layout(location = 1) in vec2 a_start;
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, start));
  glVertexAttribDivisor(1, 1);

  // layout(location = 2) in vec2 a_end;
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, end));
  glVertexAttribDivisor(2, 1);

  // layout(location = 3) in float a_thickness;
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, thickness));
  glVertexAttribDivisor(3, 1);

  // layout(location = 4) in vec4 a_color;
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, color));
  glVertexAttribDivisor(4, 1);

  // Clean up
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ViewportLayer::OnUpdate() {

  float deltaTime = static_cast<float>(Window::GetDeltaTime());
  float animationSpeed = 6.0f;

  m_Grid.Update(m_Viewport.size, m_Viewport.min, m_Viewport.max);
  m_Camera.Update((uint32_t)m_Viewport.size.x, (uint32_t)m_Viewport.size.y);

  // Update the instance buffer
  if (m_Registry->HasChanged<CBone>()) {
    m_Registry->ClearChanged<CBone>();
  }

  const ImVec2 &mouse = m_Grid.GetMouseCoords();

  auto bones = m_Registry->Get<CBone>();

  m_Bones.clear();
  m_Bones.reserve(bones.size());

  for (auto b : bones) {
    if (b->Intersects(mouse.x, mouse.y))
      b->color = glm::mix(b->color, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
                          animationSpeed * deltaTime);
    else
      b->color =
          glm::mix(b->color, glm::vec4(1.0f), animationSpeed * deltaTime);

    m_Bones.emplace_back(*b);
  }
}

void ViewportLayer::OnRender() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);
  ImGui::PopStyleVar();

  ImVec2 viewport = ImGui::GetContentRegionAvail();
  ImVec2 windowPosition = ImGui::GetWindowPos();

  m_Viewport.min = windowPosition + ImGui::GetWindowContentRegionMin();
  m_Viewport.max = windowPosition + ImGui::GetWindowContentRegionMax();

  OnUpdate();

  // Draw instances
  {
    if (m_FrameBuffer == 0 || viewport != m_Viewport.size) {
      Window::GenerateFrameBuffer(viewport, m_FrameBuffer, m_DepthBuffer,
                                  m_ColorAttachment);
      glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y);
      m_Viewport.size = viewport;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_Bones.size() * sizeof(CBone),
                 m_Bones.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    m_Shader.bind("default");
    m_Shader.setUniformMatrix4fv("u_ViewProjection",
                                 m_Camera.GetViewProjectionMatrix());

    GLsizei instanceCount = (GLsizei)m_Bones.size();
    if (instanceCount > 0)
      glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instanceCount);

    m_Shader.unbind();
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  m_Grid.Render(m_Viewport.size, m_Viewport.min);
  ImGui::SetCursorScreenPos(m_Viewport.min);
  ImGui::Image((void *)(intptr_t)m_ColorAttachment, m_Viewport.size);

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {
  std::vector<uint8_t> buffer(sizeof(m_Camera.Position) +
                              sizeof(m_Camera.Zoom));

  std::memcpy(buffer.data(), glm::value_ptr(m_Camera.Position),
              sizeof(m_Camera.Position));
  std::memcpy(buffer.data() + sizeof(m_Camera.Position), &m_Camera.Zoom,
              sizeof(m_Camera.Zoom));

  serializer.Stage("camera", buffer.data(),
                   static_cast<uint64_t>(buffer.size()));
}

void ViewportLayer::Restore(Serializer &serializer) {
  std::vector<uint8_t> buffer = serializer.Get("camera");

  if (buffer.size() < sizeof(m_Camera.Position) + sizeof(m_Camera.Zoom))
    return;

  std::memcpy(glm::value_ptr(m_Camera.Position), buffer.data(),
              sizeof(m_Camera.Position));
  std::memcpy(&m_Camera.Zoom, buffer.data() + sizeof(m_Camera.Position),
              sizeof(m_Camera.Zoom));
}
