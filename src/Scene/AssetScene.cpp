#include "AssetScene.h"
#include <imgui_internal.h>

void AssetScene::onDraw(float deltaTime)
{
  ImGui::ShowDemoWindow();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_MenuBar);

  this->DrawMenu();

  ImGui::End();
}

void AssetScene::DrawMenu()
{
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Assets"))
    {
      if (ImGui::MenuItem("Import assets"))
      {
        // auto callback = [this](std::vector<std::string> paths)
        // { this->assetManager->importAssets(paths); };

        // NativeFileDialog::Filters filters[1] = {{"Images", "png,jpg,jpeg"}};

        // this->app->AsyncTask(callback, NativeFileDialog::SelectFiles, this->app->getWindow(), filters, 1);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}
