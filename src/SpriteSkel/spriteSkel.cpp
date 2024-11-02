#include "spriteSkel.h"

#include <stdio.h>

SpriteSkel::SpriteSkel() : mainMenu()
{
}

void SpriteSkel::onInitialize()
{
}

void SpriteSkel::onInput(SDL_Event *event, float deltaTime)
{
}

void SpriteSkel::onUpdate(float deltaTime)
{
  // printf("FPS: %i\n", (int)(1 / deltaTime));
}

void SpriteSkel::onDraw(float deltaTime)
{
  this->mainMenu.draw();
}

void SpriteSkel::onCleanUp()
{
}
