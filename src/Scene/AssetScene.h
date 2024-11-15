#pragma once

#include "imgui.h"

#include "App.h"
#include "Scene/Scene.h"
#include "Asset/Asset.h"

class AssetScene : public Scene
{
private:
  App *app;
  Asset asset;

public:
  AssetScene(App *app) : app(app) {}
  ~AssetScene() {}

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void DrawMenu();
};