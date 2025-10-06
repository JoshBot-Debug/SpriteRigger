#include "State.h"
#include "Window/Window.h"

#include "Layers/AssetLayer.h"
#include "Layers/HierarchyLayer.h"
#include "Layers/InitializeLayer.h"
#include "Layers/ViewportLayer.h"

#include "Application/Rigger.h"
#include "Components/Hierarchy.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

int main(int argc, char **argv) {

  Window::Init();

  State state;

  ServiceLocator::Register<Rigger>();
  ServiceLocator::Register<ECS::Registry>();

  while (state.IsApplicationRunning()) {
    ServiceLocator::Register<ECS::SystemManager>();

    bool initialized = state.IsInitialized();

    Window::Options options = {
        .title = "Sprite Rigger",
        .darkMode = IsDarkMode(),
    };

    if (!initialized) {
      options.width = 720;
      options.height = 480;
      options.enableTitleBar = false;
    }

    Window window(options);

    if (initialized) {
      Window::RegisterShortcut({
          .ctrl = true,
          .key = ImGuiKey_N,
          .callback =
              [&state](Window *window) {
                if (state.New())
                  window->Quit();
              },
      });

      Window::RegisterShortcut({
          .ctrl = true,
          .key = ImGuiKey_O,
          .callback =
              [&state](Window *window) {
                if (state.Open())
                  window->Quit();
              },
      });

      Window::RegisterShortcut({
          .ctrl = true,
          .key = ImGuiKey_S,
          .callback =
              [&state](Window *window) {
                (void *)(window);
                state.Save();
              },
      });

      Window::RegisterShortcut({
          .ctrl = true,
          .shift = true,
          .key = ImGuiKey_S,
          .callback =
              [&state](Window *window) {
                (void *)(window);
                state.SaveAs();
              },
      });

      Window::RegisterShortcut({
          .ctrl = true,
          .key = ImGuiKey_Q,
          .callback =
              [&state](Window *window) {
                window->Quit();
                state.Quit();
              },
      });

      window.SetMenubar([&state, &window]() {
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
    }

    glfwSetWindowUserPointer(Window::GetWindow(), &state);

    glfwSetWindowCloseCallback(Window::GetWindow(), [](GLFWwindow *window) {
      auto *state = static_cast<State *>(glfwGetWindowUserPointer(window));
      state->Quit();
    });

    if (!initialized) {
      Window::RegisterShortcut({
          .key = ImGuiKey_Escape,
          .callback =
              [&state](Window *window) {
                window->Quit();
                state.Quit();
              },
      });
      window.PushLayer<InitializeLayer>(&state);
    } else {
      window.PushLayer(state.Register(std::make_shared<ViewportLayer>(&state)));
      window.PushLayer(
          state.Register(std::make_shared<HierarchyLayer>(&state)));
      window.PushLayer(state.Register(std::make_shared<AssetLayer>(&state)));
      state.Restore();
    }

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowMenuButtonPosition = ImGuiDir_None;

    window.Run();
    ServiceLocator::Unregister<ECS::SystemManager>();
  }

  ServiceLocator::Unregister<ECS::Registry>();
  ServiceLocator::Unregister<Rigger>();

  Window::Shutdown();
  return 0;
}