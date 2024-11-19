#pragma once

#include "imgui.h"

#include "Scene/Scene.h"

class AssetScene : public Scene
{
private:

public:
  AssetScene() = default;
  ~AssetScene() = default;

  void onDraw(float deltaTime) override;
  
  void DrawMenu();
};