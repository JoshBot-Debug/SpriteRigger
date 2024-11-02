#include "SDL3/SDL.h"
#include "ShadowFrame/shadowFrame.h"

int main(int argc, char *argv[])
{
  ShadowFrame shadowFrame;

  shadowFrame.setWindowTitle("Shadow Frame");
  shadowFrame.setVSync(1);
  shadowFrame.setWindowDimensions(800, 600);
  shadowFrame.setBackgroundColor(255, 255, 255, SDL_ALPHA_OPAQUE);
  shadowFrame.setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf");
  shadowFrame.open();

  return 0;
}