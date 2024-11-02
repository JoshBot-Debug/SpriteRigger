#include "spriteRigger.h"

#include <stdio.h>

SpriteRigger::SpriteRigger()
{
  this->mainMenu.setApplication(this);
}

void SpriteRigger::onInitialize()
{
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
}

void SpriteRigger::onCleanUp()
{
}
