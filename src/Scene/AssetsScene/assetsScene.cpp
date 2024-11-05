#include "assetsScene.h"
#include <imgui_internal.h>

void AssetsScene::onInitialize()
{
}

void AssetsScene::onInput(SDL_Event *event, float deltaTime)
{
}

void AssetsScene::onUpdate(float deltaTime)
{
}

void AssetsScene::onDraw(float deltaTime)
{
  ImGui::ShowDemoWindow();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_MenuBar);

  this->DrawMenu();

  ImGui::End();
}

void AssetsScene::onCleanUp()
{
}

void AssetsScene::DrawMenu()
{
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Assets"))
    {
      if (ImGui::MenuItem("Import assets"))
      {
        Assets *assets = &this->assets;
        
        auto callback = [assets](std::vector<std::string> paths)
        { assets->importAssets(paths); };

        NativeFileDialog::Filters filters[1] = {{"Images", "png,jpg,jpeg"}};

        this->application->AsyncTask(callback, NativeFileDialog::SelectFiles, this->application->getWindow(), filters, 1);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}
