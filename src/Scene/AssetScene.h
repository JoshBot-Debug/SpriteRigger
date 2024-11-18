#pragma once

#include "imgui.h"

#include "Scene/Scene.h"

class AssetScene : public Scene
{
private:

public:
  AssetScene() = default;
  ~AssetScene() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void DrawMenu();
};