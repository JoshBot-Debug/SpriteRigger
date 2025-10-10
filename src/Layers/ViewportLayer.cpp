#include "ViewportLayer.h"

#include "Application/Components.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

#include "ECS/Utility.h"

ViewportLayer::ViewportLayer(State *state)
    : m_State(state), m_Grid(&m_Camera) {}

void ViewportLayer::OnAttach() {
  m_Registry = ServiceLocator::Get<ECS::Registry>();
  m_System = ServiceLocator::Get<ECS::SystemManager>();

  m_HoverSystem = m_System->Register<HoverSystem>();
  m_SelectSystem = m_System->Register<SelectSystem>();
  m_DragSystem = m_System->Register<DragSystem>();
  m_BoneRenderSystem = m_System->Register<BoneRenderSystem>();
  m_ColorInterpolationSystem = m_System->Register<ColorInterpolationSystem>();

  m_HoverSystem->Initialize(m_Registry.get(), &m_Grid, &m_Camera);
  m_SelectSystem->Initialize(m_Registry.get(), &m_Grid, &m_Camera);
  m_DragSystem->Initialize(m_Registry.get(), &m_Grid, &m_Camera);
  m_BoneRenderSystem->Initialize(m_Registry.get(), &m_Shader, &m_Camera);
  m_ColorInterpolationSystem->Initialize(m_Registry.get(), &m_Grid, &m_Camera);
}

void ViewportLayer::OnRender() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);
  ImGui::PopStyleVar();

  ImGuiIO &io = ImGui::GetIO();
  ImVec2 viewport = ImGui::GetContentRegionAvail();
  ImVec2 windowPosition = ImGui::GetWindowPos();

  m_Viewport.min = windowPosition + ImGui::GetWindowContentRegionMin();
  m_Viewport.max = windowPosition + ImGui::GetWindowContentRegionMax();

  {
    m_Grid.Update(m_Viewport.size, m_Viewport.min, m_Viewport.max);
    m_Camera.Update((uint32_t)m_Viewport.size.x, (uint32_t)m_Viewport.size.y);

    m_SystemData.deltaTime = static_cast<float>(Window::GetDeltaTime());
    m_SystemData.mouse = m_Grid.GetMouseCoords();
    m_SystemData.isDragging = ImGui::IsMouseDragging(ImGuiMouseButton_Left);
    m_SystemData.isMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    m_SystemData.isCtrlDown = ImGui::IsKeyDown(ImGuiKey_LeftCtrl);
    m_SystemData.deltaMouse = m_Grid.GetDeltaMouseCoords();
    m_SystemData.isMouseClicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    m_System->Update<HoverSystem>(&m_SystemData);
    m_System->Update<SelectSystem>(&m_SystemData);
    // m_System->Update<DragSystem>(&m_SystemData);
    m_System->Update<ColorInterpolationSystem>(&m_SystemData);
  }

  ResizeFramebuffer(viewport);

  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_System->Update<BoneRenderSystem>(&m_SystemData);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  m_Grid.Render(m_Viewport.size, m_Viewport.min);
  ImGui::SetCursorScreenPos(m_Viewport.min);
  ImGui::Image((void *)(intptr_t)m_ColorAttachment, m_Viewport.size);
  ImGui::End();
}

void ViewportLayer::OnDetach() {
  m_State = nullptr;
  m_System->Free();
}

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

void ViewportLayer::ResizeFramebuffer(ImVec2 viewport) {
  if (m_FrameBuffer == 0 || viewport != m_Viewport.size) {
    Window::GenerateFrameBuffer(viewport, m_FrameBuffer, m_DepthBuffer,
                                m_ColorAttachment);
    glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y);
    m_Viewport.size = viewport;
  }
}
