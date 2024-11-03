#include "spriteRigger.h"

#include <stdio.h>

void SpriteRigger::onInitialize()
{
  this->viewport.setBackgroundColor(0, 0, 0, SDL_ALPHA_OPAQUE);
  this->viewport.setDimensions(1080, 720);
  this->viewport.setRenderer(this->renderer);

  this->mainMenu.setApplication(this);
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
}

void SpriteRigger::onUpdate(float deltaTime)
{
  // printf("FPS: %i\n", (int)(1 / deltaTime));
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->mainMenu.onDraw();

  ImGui::Begin("Window");
  ImGui::Text("FPS: %i", (int)(1 / deltaTime));
  ImGui::End();

  auto renderViewport = [this, deltaTime]()
  {
    SDL_FRect rect = {0, 0, 100, 100};
    SDL_SetRenderDrawColor(this->renderer, 100, 50, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->renderer, &rect);
  };

  this->viewport.draw("Viewport", renderViewport);
}

void SpriteRigger::onCleanUp()
{
}
