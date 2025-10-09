#include "Grid.h"

#include <iostream>
#include <stdio.h>

Grid::Grid(OrthographicCamera *camera) : m_Camera(camera) {}

void Grid::Update(ImVec2 viewportSize, ImVec2 viewportMin, ImVec2 viewportMax) {
  if (ImGui::IsMouseHoveringRect(viewportMin, viewportMax)) {
    ImGuiIO &io = ImGui::GetIO();

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
      float dx = -io.MouseDelta.x * m_Camera->Zoom * 2.0f / viewportSize.x;
      float dy = io.MouseDelta.y * m_Camera->Zoom * 2.0f / viewportSize.y;
      m_Camera->Translate(dx, dy);
    }

    if (io.MouseWheel != 0.0f)
      m_Camera->Zoom *= (1.0f - io.MouseWheel * 0.1f);
  }
}

void Grid::Render(ImVec2 viewportSize, ImVec2 viewportMin) {

  float halfWidth = m_Camera->Aspect * m_Camera->Zoom;
  float halfHeight = m_Camera->Zoom;

  float left = m_Camera->Position.x - halfWidth;
  float right = m_Camera->Position.x + halfWidth;
  float bottom = m_Camera->Position.y - halfHeight;
  float top = m_Camera->Position.y + halfHeight;

  float scaleX = viewportSize.x / (right - left);
  float scaleY = viewportSize.y / (top - bottom);

  float lineStepX = std::max(1.0f, std::ceil(m_MinLineSpacing / scaleX));
  float lineStepY = std::max(1.0f, std::ceil(m_MinLineSpacing / scaleY));

  float offsetX = viewportMin.x - left * scaleX;
  float offsetY = viewportMin.y + viewportSize.y + bottom * scaleY;

  if (!m_Font)
    m_Font = ImGui::GetFont();

  // Draw info
  {
    ImVec2 mouseScreen = ImGui::GetMousePos();
    ImVec2 mouseInViewport =
        ImVec2(mouseScreen.x - viewportMin.x, mouseScreen.y - viewportMin.y);

    m_PreviousMouse = m_Mouse;
    m_Mouse.x = left + (mouseInViewport.x / viewportSize.x) * (right - left);
    m_Mouse.y =
        bottom + ((viewportSize.y - mouseInViewport.y) / viewportSize.y) *
                     (top - bottom);

    ImGui::PushFont(m_Font, m_FontSize);

    ImGui::SetCursorScreenPos(
        ImVec2(viewportMin.x + 16.0f + 4.0f, viewportMin.y + 16.0f + 4.0f));

    ImGui::Text("Camera (%.2f, %.2f, %.2f)", m_Camera->Position.x,
                m_Camera->Position.y, m_Camera->Zoom);

    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(ImVec2(cursor.x + 16.0f + 4.0f, cursor.y + 4.0f));
    ImGui::Text("Mouse (%.2f, %.2f)", m_Mouse.x, m_Mouse.y);

    ImGui::PopFont();
  }

  ImDrawList *drawList = ImGui::GetWindowDrawList();

  auto WorldToScreen = [&](float x, float y) -> ImVec2 {
    return ImVec2(offsetX + x * scaleX, offsetY - y * scaleY);
  };

  // Vertical lines
  float firstX = std::floor(left / m_UnitLength) * m_UnitLength;
  for (float x = firstX; x <= right; x += m_UnitLength) {
    if ((int)std::round(x / lineStepX) * lineStepX != (int)x)
      continue;

    ImVec2 p0 = WorldToScreen(x, bottom);
    ImVec2 p1 = WorldToScreen(x, top);

    ImU32 color = (x == 0.0f) ? m_AxisColor : m_LineColor;

    drawList->AddLine(p0, p1, color, 1.0f);

    char buf[16];
    snprintf(buf, sizeof(buf), "%.0f", x);
    drawList->AddText(m_Font, m_FontSize,
                      ImVec2(p0.x + 4, p0.y - m_FontSize - 4), m_AxisColor,
                      buf);
  }

  // Horizontal lines
  float firstY = std::floor(bottom / m_UnitLength) * m_UnitLength;
  for (float y = firstY; y <= top; y += m_UnitLength) {
    if ((int)std::round(y / lineStepY) * lineStepY != (int)y)
      continue;

    ImVec2 p0 = WorldToScreen(left, y);
    ImVec2 p1 = WorldToScreen(right, y);

    ImU32 color = (y == 0.0f) ? m_AxisColor : m_LineColor;

    drawList->AddLine(p0, p1, color, 1.0f);

    char buf[16];
    snprintf(buf, sizeof(buf), "%.0f", y);
    drawList->AddText(m_Font, m_FontSize, ImVec2(p0.x + 4, p0.y + 4),
                      m_AxisColor, buf);
  }
}