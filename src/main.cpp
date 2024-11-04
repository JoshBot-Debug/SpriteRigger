#include "SDL3/SDL.h"
#include "spriteRigger.h"

int main(int argc, char *argv[])
{
  SpriteRigger SpriteRigger;

  SpriteRigger.setWindowTitle("SpriteRigger");
  SpriteRigger.setVSync(1);
  SpriteRigger.setWindowDimensions({1080, 720});
  SpriteRigger.setBackgroundColor({30, 30, 30, 255});
  SpriteRigger.setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf", 18);
  SpriteRigger.addFont("assets/fonts/roboto/Roboto-Regular.ttf", 20);
  SpriteRigger.setTheme(Theme::DARK);
  SpriteRigger.open();

  // Registry registry;

  // Entity *bone1 = registry.createEntity("Bone");
  // bone1->add<PropertiesComponent>("Bone 1", 1);
  // bone1->add<MeshComponent>(Vec2{100, 100});
  // bone1->add<TransformComponent>(Vec2{100, 100});
  // bone1->add<ColorComponent>(Vec4{255, 255, 255, 255});

  // Entity *bone2 = registry.createEntity("Bone");
  // bone2->add<PropertiesComponent>("Bone 2", 2);
  // bone2->add<MeshComponent>(Vec2{100, 100});
  // bone2->add<TransformComponent>(Vec2{100, 300});
  // bone2->add<ColorComponent>(Vec4{255, 0, 0, 255});

  // registry.free<PropertiesComponent, MeshComponent, TransformComponent, ColorComponent>();

  return 0;
}