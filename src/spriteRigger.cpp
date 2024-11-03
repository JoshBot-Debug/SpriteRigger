#include "spriteRigger.h"

#include <stdio.h>


void SpriteRigger::onInitialize()
{
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
}

void SpriteRigger::onCleanUp()
{
}
