#include "ShadowFrame/shadow_frame.h"

int main(int argc, char *argv[])
{
  ShadowFrame app;

  app.setWindowTitle("Shadow Frame");
  app.setWindowDimensions(800, 600);
  app.launch();

  return 0;
}