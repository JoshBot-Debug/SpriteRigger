#include "State.h"
#include "Window/Window.h"

#include "Layers/AssetLayer.h"
#include "Layers/HierarchyLayer.h"
#include "Layers/InitializeLayer.h"
#include "Layers/ViewportLayer.h"

#include "Utility.h"

int main(int argc, char **argv) {

  State state;

  std::vector<std::shared_ptr<SerializableLayer>> layers = {
      state.Register(std::make_shared<ViewportLayer>(&state)),
      state.Register(std::make_shared<HierarchyLayer>(&state)),
      state.Register(std::make_shared<AssetLayer>(&state)),
  };

  while (state.IsApplicationRunning()) {
    bool initialized = state.IsInitialized();

    Window::Options options = {
        .title = "State",
        .darkMode = IsDarkMode(),
    };

    if (!initialized) {
      options.width = 720;
      options.height = 480;
      options.enableTitleBar = false;
    }

    Window window(options);

    glfwSetWindowUserPointer(Window::GetWindow(), &state);

    glfwSetWindowCloseCallback(Window::GetWindow(), [](GLFWwindow *window) {
      static_cast<State *>(glfwGetWindowUserPointer(window))->Quit();
    });

    window.SetMenubar([&state, &window]() {
      bool ctrl = ImGui::IsKeyDown(ImGuiMod_Ctrl);
      bool shift = ImGui::IsKeyDown(ImGuiMod_Shift);
      bool n = ImGui::IsKeyPressed(ImGuiKey_N, false);
      bool o = ImGui::IsKeyPressed(ImGuiKey_O, false);
      bool s = ImGui::IsKeyPressed(ImGuiKey_S, false);
      bool q = ImGui::IsKeyPressed(ImGuiKey_Q, false);

      if (ctrl && n)
        if (state.New())
          window.Quit();

      if (ctrl && o)
        if (state.Open())
          window.Quit();

      if (ctrl && s)
        state.Save();

      if (shift && ctrl && s)
        state.SaveAs();

      if (ctrl && q) {
        window.Quit();
        state.Quit();
      }

      if (ImGui::BeginMenu("File")) {

        if (ImGui::MenuItem("New", "Crtl N"))
          if (state.New())
            window.Quit();

        if (ImGui::MenuItem("Open", "Crtl O"))
          if (state.Open())
            window.Quit();

        if (ImGui::BeginMenu("Recent")) {

          for (const auto &project : state.GetRecentProjects()) {
            std::filesystem::path filePath = project;
            if (ImGui::MenuItem(Ellipsize(RelativeHomePath(project), 400,
                                          EllipsizeType::START)
                                    .c_str())) {
              state.Open(project);
              window.Quit();
            }
          }

          ImGui::EndMenu();
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Save", "Crtl S"))
          state.Save();

        if (ImGui::MenuItem("Save As", "Shift Crtl S"))
          state.SaveAs();

        ImGui::Separator();

        if (ImGui::MenuItem("Quit", "Crtl Q")) {
          window.Quit();
          state.Quit();
        }

        ImGui::EndMenu();
      }
    });

    if (!initialized)
      window.PushLayer<InitializeLayer>(&state);
    else
      for (auto &layer : layers)
        window.PushLayer(layer);

    window.Run();
  }

  return 0;
}