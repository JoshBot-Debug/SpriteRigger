#include "shadowFrame.h"
#include <stdio.h>

void ShadowFrame::onInitialize()
{
  // This will not work at the momemnt becase TextManager does not have any DefaultText.
  // Was removed when I removed showFPS();
  // This needs to be reworked in a better way, need to set fonts and texts at the start of the application
  // and then use them throughout the app.
  this->text = new Text;
  this->text->content = "Hi there";
  this->text->textName = "DefaultText";
  this->text->x = 0;
  this->text->y = 300;
}

void ShadowFrame::onInput(SDL_Event *event, float deltaTime)
{
}

void ShadowFrame::onUpdate(float deltaTime)
{
  // printf("MAIN FPS: %i\n", (int)(1 / deltaTime));=
}

void ShadowFrame::onDraw(float deltaTime)
{
  this->textManager->render(this->text);
}

void ShadowFrame::onCleanUp()
{
  delete this->text;
}
