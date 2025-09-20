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

    window.SetMenubar([&state]() {
      bool ctrl = ImGui::IsKeyDown(ImGuiMod_Ctrl);
      bool shift = ImGui::IsKeyDown(ImGuiMod_Shift);
      bool n = ImGui::IsKeyPressed(ImGuiKey_N, false);
      bool o = ImGui::IsKeyPressed(ImGuiKey_O, false);
      bool s = ImGui::IsKeyPressed(ImGuiKey_S, false);

      if (ctrl && n)
        state.New();

      if (ctrl && o)
        state.Open();

      if (ctrl && s)
        state.Save();

      if (shift && ctrl && s)
        state.SaveAs();

      if (ImGui::BeginMenu("File")) {

        if (ImGui::MenuItem("New", "Crtl N"))
          state.New();

        if (ImGui::MenuItem("Open", "Crtl O"))
          state.Open();

        if (ImGui::BeginMenu("Recent")) {
          if (ImGui::MenuItem(Ellipsize("xyz.sprig", 230).c_str()))
            state.Open("/path/to/file.sprig");

          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Save", "Crtl S"))
          state.Save();

        if (ImGui::MenuItem("Save As", "Shift Crtl S"))
          state.SaveAs();

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