#pragma once

#include "imgui.h"
#include "App.h"
#include "Scene/Scene.h"
#include "Component/Component.h"
#include "Assets/Assets.h"

class AssetsScene : public Scene
{
private:
  App *app;
  Assets assets;
  
public:
  AssetsScene(App *app) : app(app) {}
  ~AssetsScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void DrawMenu();
};