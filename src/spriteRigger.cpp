#include "spriteRigger.h"

#include <stdio.h>

void SpriteRigger::onInitialize()
{
  this->viewport.setBackgroundColor({0, 0, 0, 255});
  this->viewport.setDimensions({1080, 720});
  this->viewport.setRenderer(this->renderer);

  this->mainMenu.setApplication(this);
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
  this->mouse.set(event->button.x, event->button.y);
}

void SpriteRigger::onUpdate(float deltaTime)
{
  auto [x, y] = this->viewport.getMousePosition(this->mouse);

  printf("Viewport mPos: x:%f y:%f\n", x, y);
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->mainMenu.onDraw();

  ImGui::Begin("Window");
  ImGui::Text("FPS: %i", (int)(1 / deltaTime));
  ImGui::End();

  this->viewport.draw("Viewport", [this, deltaTime]()
                      { this->onDrawViewport(deltaTime); });
}

void SpriteRigger::onCleanUp()
{
}

void SpriteRigger::onDrawViewport(float deltaTime)
{
  SDL_FRect rect = {0, 0, 100, 100};
  SDL_SetRenderDrawColor(this->renderer, 100, 50, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(this->renderer, &rect);
}
