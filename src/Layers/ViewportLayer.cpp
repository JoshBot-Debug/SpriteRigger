#include "ViewportLayer.h"

#include "Application/Components.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

const std::string EXE_DIRECTORY = GetExecutableDirectory();

ViewportLayer::ViewportLayer(State *state) : m_State(state) {}

void ViewportLayer::OnAttach() {
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

void ViewportLayer::OnUpdate(float deltaTime) {

  auto registry = ServiceLocator::Get<Registry>();

  if (!registry->HasChanged<CBone>())
    return;

  registry->ClearChanged<CBone>();

  std::vector<CBone *> bones = registry->Get<CBone>();

  m_Bones.resize(bones.size());

  for (auto &bone : bones)
    m_Bones.emplace_back(*bone);
}

void ViewportLayer::OnRender() {
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);

  ImVec2 viewport = ImGui::GetContentRegionAvail();

  // Prepare the framebuffer
  if (m_FrameBuffer == 0 || viewport.x != m_ViewportSize.x ||
      viewport.y != m_ViewportSize.y)
    Window::GenerateFrameBuffer(viewport, m_FrameBuffer, m_DepthBuffer,
                                m_ColorAttachment);

  glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
  glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Upload buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, m_Bones.size() * sizeof(CBone), m_Bones.data(),
               GL_DYNAMIC_DRAW);

  glBindVertexArray(m_VAO);

  // Set uniforms
  m_Shader.bind("default");
  m_Shader.setUniform2f("u_screenSize", glm::vec2(viewport.x, viewport.y));

  // Draw
  GLsizei instanceCount = (GLsizei)m_Bones.size();
  if (instanceCount > 0)
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instanceCount);

  m_Shader.unbind();
  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ImGui::Image((void *)(intptr_t)m_ColorAttachment, viewport);

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {}

void ViewportLayer::Restore(Serializer &serializer) {}
