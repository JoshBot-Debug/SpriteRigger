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
  m_Camera.Update();

  // Update the instance buffer
  {
    auto registry = ServiceLocator::Get<Registry>();

    if (!registry->HasChanged<CBone>())
      return;

    registry->ClearChanged<CBone>();

    std::vector<CBone *> bones = registry->Get<CBone>();

    m_Bones.resize(bones.size());

    for (auto &bone : bones)
      m_Bones.emplace_back(*bone);
  }
}

void ViewportLayer::OnRender() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);
  ImGui::PopStyleVar();

  ImVec2 viewport = ImGui::GetContentRegionAvail();

  m_Camera.OnResize((uint32_t)viewport.x, (uint32_t)viewport.y);
  m_Camera.Update();

  if (ImGui::IsWindowFocused()) {
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
      ImGuiIO &io = ImGui::GetIO();
      float dx = -io.MouseDelta.x * m_Camera.Zoom * 2.0f /
                 ImGui::GetContentRegionAvail().x;
      float dy = io.MouseDelta.y * m_Camera.Zoom * 2.0f /
                 ImGui::GetContentRegionAvail().y;

      m_Camera.Translate(dx, dy);
    }

    if (Window::GetMouseScroll().y != 0.0f)
      m_Camera.Zoom *= (1.0f - Window::GetMouseScroll().y * 0.1f);
  }

  // Draw instances
  {
    // Prepare the framebuffer
    if (m_FrameBuffer == 0 || viewport != m_ViewportSize) {
      Window::GenerateFrameBuffer(viewport, m_FrameBuffer, m_DepthBuffer,
                                  m_ColorAttachment);

      glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Upload buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, m_Bones.size() * sizeof(CBone),
                 m_Bones.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    // Set uniforms
    m_Shader.bind("default");
    m_Shader.setUniformMatrix4fv("u_ViewProjection",
                                 m_Camera.GetViewProjectionMatrix());

    // Draw
    GLsizei instanceCount = (GLsizei)m_Bones.size();
    if (instanceCount > 0)
      glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instanceCount);

    m_Shader.unbind();
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((void *)(intptr_t)m_ColorAttachment, viewport);
  }

  {
    ImGui::PushFont(Window::GetFont("RobotoLight"), 14.0f);

    ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImGui::SetCursorScreenPos(windowPos + contentMin + ImVec2(16, 16));

    ImGui::Text("Camera (%.2f, %.2f, %.2f)", m_Camera.Position.x,
                m_Camera.Position.y, m_Camera.Zoom);

    ImVec2 mousePos = GetViewportMouse(m_Camera);
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(16, 0));
    ImGui::Text("Mouse (%.2f, %.2f)", mousePos.x, mousePos.y);

    ImGui::PopFont();
  }

  {
    // Draw grid
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
    ImVec2 origin = windowPos + contentMin;

    float fontSize = 14.0f;
    ImFont *font = Window::GetFont("RobotoLight");

    float aspect = (float)m_Camera.ViewportWidth / m_Camera.ViewportHeight;
    float halfWidth = aspect * m_Camera.Zoom;
    float halfHeight = 1.0f * m_Camera.Zoom;

    float left = m_Camera.Position.x - halfWidth;
    float right = m_Camera.Position.x + halfWidth;
    float bottom = m_Camera.Position.y - halfHeight;
    float top = m_Camera.Position.y + halfHeight;

    float gridSpacing = 1.0f; // 1 world unit

    // compute approximate pixels per world unit
    float pixelsPerUnitX = viewport.x / (right - left);
    float pixelsPerUnitY = viewport.y / (top - bottom);

    // minimum pixels between labels
    float minLabelSpacing = 50.0f;

    ImU32 defaultColor = IM_COL32(100, 100, 100, 100);
    ImU32 centerColor = IM_COL32(200, 200, 200, 200);
    float labelStepX =
        std::max(1.0f, std::ceil(minLabelSpacing / pixelsPerUnitX));
    float labelStepY =
        std::max(1.0f, std::ceil(minLabelSpacing / pixelsPerUnitY));

    // Convert world coordinates to screen pixels
    auto WorldToScreen = [&](float x, float y) -> ImVec2 {
      float sx = ((x - left) / (right - left)) * viewport.x;
      float sy = ((y - bottom) / (top - bottom)) * viewport.y;
      return ImVec2(origin.x + sx, origin.y + viewport.y - sy);
    };

    // Vertical lines
    float firstX = std::floor(left / gridSpacing) * gridSpacing;
    for (float x = firstX; x <= right; x += gridSpacing) {
      ImVec2 p0 = WorldToScreen(x, bottom);
      ImVec2 p1 = WorldToScreen(x, top);

      ImU32 color = (x == 0.0f) ? centerColor : defaultColor;

      drawList->AddLine(p0, p1, color, 1.0f);

      if (std::fmod(x, labelStepX) == 0.0f) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.0f", x);
        ImVec2 textPos = ImVec2(p0.x + 4, p0.y - fontSize - 4); // adjust offset
        drawList->AddText(font, fontSize, textPos, centerColor, buf);
      }
    }

    // Horizontal lines
    float firstY = std::floor(bottom / gridSpacing) * gridSpacing;
    for (float y = firstY; y <= top; y += gridSpacing) {
      ImVec2 p0 = WorldToScreen(left, y);
      ImVec2 p1 = WorldToScreen(right, y);

      ImU32 color = (y == 0.0f) ? centerColor : defaultColor;

      drawList->AddLine(p0, p1, color, 1.0f);

      if (std::fmod(y, labelStepY) == 0.0f) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.0f", y);
        drawList->AddText(font, fontSize, ImVec2(p0.x + 4, p0.y + 4),
                          centerColor, buf);
      }
    }
  }

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {}

void ViewportLayer::Restore(Serializer &serializer) {}
