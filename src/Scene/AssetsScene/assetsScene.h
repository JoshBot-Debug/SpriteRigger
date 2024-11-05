#pragma once

#include "imgui.h"
#include "application.h"
#include "Scene/scene.h"
#include "Entity/components.h"
#include "Assets/assets.h"

class AssetsScene : public Scene
{
private:
  Assets assets;
  
public:
  AssetsScene(Application *application) : Scene(application) {}
  ~AssetsScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void DrawMenu();
};