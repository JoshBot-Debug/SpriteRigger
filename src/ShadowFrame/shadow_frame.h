#include "Application/application.h"
#include "Button/button.h"

class ShadowFrame : public Application
{
private:
  Button *button;

public:
  ShadowFrame();
  ~ShadowFrame();

  void onDraw(float deltaTime) override;
  void onInput(SDL_Event event) override;
};