#include "Application/application.h"

class ShadowFrame : public Application
{
public:
  ShadowFrame() {}
  ~ShadowFrame() override {}

  void onDraw(float deltaTime) override;
  void onInput(SDL_Event event) override;
};