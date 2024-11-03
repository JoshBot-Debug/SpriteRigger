#include "spriteRigger.h"

#include <stdio.h>

void SpriteRigger::onInitialize()
{
  this->viewport.setRenderer(this->renderer);
  this->viewport.resize(1080, 720);
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

  ImGui::Begin("Dock Window");
  ImGui::Text("I am dockable");
  ImGui::End();

  auto renderViewport = [this, deltaTime]()
  {
    // ImGui::Text("FPS: %i", (int)(1 / deltaTime));

    SDL_FRect rect = {0, 0, 100, 100};
    SDL_SetRenderDrawColor(this->renderer, 100, 50, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->renderer, &rect);
  };

  this->viewport.draw("Viewport", renderViewport);
}

void SpriteRigger::onCleanUp()
{
}
