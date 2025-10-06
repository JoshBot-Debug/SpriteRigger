#pragma once

#include "Camera/OrthographicCamera.h"
#include "imgui.h"

/**
 * @brief Renders a 2D grid in the viewport using ImGui's draw list.
 *
 * The Grid class draws a coordinate system grid that adapts to the
 * camera zoom, aspect ratio, and viewport size. It supports customizable
 * grid spacing, unit size, colors, and text rendering using an ImGui font.
 */
class Grid {
private:
  /// Pointer to the orthographic camera used for world-to-screen conversion.
  OrthographicCamera *m_Camera = nullptr;

  /// Font size (in pixels) for grid labels.
  float m_FontSize = 14.0f;

  /// ImGui font pointer used for drawing text labels.
  ImFont *m_Font = nullptr;

  /// Minimum number of pixels between grid lines (prevents clutter).
  float m_MinLineSpacing = 25.0f;

  /// World space distance represented by one grid step.
  float m_UnitLength = 1.0f;

  /// Color of normal grid lines.
  ImU32 m_LineColor = IM_COL32(100, 100, 100, 100);

  /// Color of origin (x=0, y=0) grid lines.
  ImU32 m_AxisColor = IM_COL32(200, 200, 200, 200);

  ImVec2 m_Mouse = ImVec2(0,0);

public:
  /**
   * @param camera Pointer to an OrthographicCamera instance.
   */
  explicit Grid(OrthographicCamera *camera);

  /**
   * @brief Update the grid state based on user input and viewport interaction.
   *
   * This method should be called every frame before Render(). It processes
   * ImGui input (e.g., mouse scroll for zooming, dragging for panning) to
   * adjust the camera or grid parameters.
   *
   * @param viewportSize The size of the viewport in pixels.
   * @param viewportMin  The top-left corner of the viewport in window
   * coordinates.
   * @param viewportMax  The bottom-right corner of the viewport in window
   * coordinates.
   */
  void Update(ImVec2 viewportSize, ImVec2 viewportMin, ImVec2 viewportMax);

  /**
   * @brief Render the grid within the given viewport.
   *
   * @param viewportSize The size of the viewport in pixels (e.g., {600, 400}).
   * @param viewportMin  The top-left corner of the viewport in window
   * coordinates.
   */
  void Render(ImVec2 viewportSize, ImVec2 viewportMin);

  /**
   * @brief Set the ImGui font for text labels.
   * @param font Pointer to an ImFont instance.
   */
  void SetFont(ImFont *font) { m_Font = font; };

  /**
   * @brief Set the minimum pixel spacing between grid lines.
   * @param spacing Pixel distance threshold before a grid line is drawn.
   */
  void SetMinimumLineSpacing(float spacing) { m_MinLineSpacing = spacing; };

  /**
   * @brief Set the grid unit size in world space.
   * @param unit Distance in world units for one grid step.
   */
  void SetUnitLength(float unit) { m_UnitLength = unit; };

  /**
   * @brief Set the colors for grid and origin lines.
   * @param gridColor Line color for non-origin grid lines.
   * @param gridOriginColor Line color for origin lines (x=0, y=0).
   */
  void SetColors(ImU32 gridColor, ImU32 gridOriginColor) {
    m_LineColor = gridColor;
    m_AxisColor = gridOriginColor;
  };

  const ImVec2 &GetMouseCoords() const { return m_Mouse; };
};
