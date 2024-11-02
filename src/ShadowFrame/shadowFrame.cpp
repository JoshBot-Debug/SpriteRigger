#include "shadowFrame.h"
#include <stdio.h>

void ShadowFrame::onInitialize()
{
}

void ShadowFrame::onInput(SDL_Event *event, float deltaTime)
{
}

void ShadowFrame::onUpdate(float deltaTime)
{
  // printf("FPS: %i\n", (int)(1 / deltaTime));
}

void ShadowFrame::onDraw(float deltaTime)
{
  ImGui::Begin("Another Window");
  ImGui::Text("Hello from another window!");
  ImGui::Button("Press Me");
  ImGui::End();
}

void ShadowFrame::onCleanUp()
{
}
