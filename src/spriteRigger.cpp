#include "spriteRigger.h"

#include <stdio.h>
#include <vector>

#include <imgui_internal.h>

struct ColorComponent
{
  Vec4 color{255, 255, 255, 255};

  float colorByPercent[4]{1.0f, 1.0f, 1.0f, 1.0f};

  void renderProperties()
  {
    ImGui::SeparatorText("Color");

    if (ImGui::ColorPicker4("Background color", colorByPercent))
    {
      this->color.x = colorByPercent[0] * 255.0f;
      this->color.y = colorByPercent[1] * 255.0f;
      this->color.z = colorByPercent[2] * 255.0f;
      this->color.w = colorByPercent[3] * 255.0f;
    }
  }
};

struct TransformComponent
{
  Vec2 position;
  Vec2 rotation;

  TransformComponent() : position(Vec2{0, 0}), rotation(Vec2{0, 0}) {}
  TransformComponent(Vec2 position) : position(position), rotation(Vec2{0, 0}) {}
  TransformComponent(Vec2 position, Vec2 rotation) : position(position), rotation(rotation) {}

  void renderProperties()
  {
    ImGui::SeparatorText("Transform");
    ImGui::InputFloat("Position X", &this->position.x, 50, 100);
    ImGui::InputFloat("Position Y", &this->position.y, 50, 100);
  }
};

struct MeshComponent
{
  Vec2 size;

  MeshComponent(Vec2 size) : size(size) {}

  void renderProperties()
  {
    ImGui::SeparatorText("Mesh");
    ImGui::SliderFloat("Width", &this->size.x, 0, 200);
    ImGui::SliderFloat("Height", &this->size.y, 0, 200);
  }

  void render(SDL_Renderer *renderer, TransformComponent *transform, ColorComponent *color)
  {

    SDL_FRect rect = {transform->position.x, transform->position.y, size.x, size.y};
    SDL_SetRenderDrawColor(renderer, color->color.x, color->color.y, color->color.z, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
  }
};

void SpriteRigger::onInitialize()
{
  this->viewport.setBackgroundColor({0, 0, 0, 255});
  this->viewport.setDimensions({1080, 720});
  this->viewport.setRenderer(this->renderer);

  this->mainMenu.setApplication(this);

  Entity *bone = this->registry.createEntity("Bone");
  bone->add<MeshComponent>(Vec2{10, 10});
  bone->add<TransformComponent>(Vec2{100, 100});
  bone->add<ColorComponent>();
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
  this->mouse.setPosition(event->button.x, event->button.y);

  switch (event->type)
  {
  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    if (event->button.button == SDL_BUTTON_LEFT)
      this->mouse.setState(MouseState::PRESSED_LEFT);
    break;
  case SDL_EVENT_MOUSE_BUTTON_UP:
    if (event->button.button == SDL_BUTTON_LEFT)
      this->mouse.setState(MouseState::RELEASED);
    break;
  default:
    break;
  }
}

void SpriteRigger::onUpdate(float deltaTime)
{
  Vec2 mouse = this->viewport.getMousePosition(this->mouse);

  for (auto entity : this->registry.entities())
  {
    if (entity->is("Bone"))
    {
      auto [mesh, transform] = entity->collect<MeshComponent, TransformComponent>();

      if (mouse.intersects(transform->position, mesh->size) && this->mouse.isPressed(MouseButton::LEFT))
      {
        float smoothing = 20.0f * deltaTime;
        transform->position = Vec2::lerp(transform->position, (mouse - transform->position), smoothing);
      }
    }
  }
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->mainMenu.onDraw();

  ImGui::Begin("Window");
  Vec2 mouse = this->viewport.getMousePosition(this->mouse);
  ImGui::Text("Mouse: x(%f) y(%f)", mouse.x, mouse.y);

  for (auto entity : this->registry.entities())
  {
    if (entity->is("Bone"))
    {
      auto [mesh, transform, color] = entity->collect<MeshComponent, TransformComponent, ColorComponent>();
      mesh->renderProperties();
      transform->renderProperties();
      color->renderProperties();
    }
  }

  ImGui::End();

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);
  this->viewport.draw("Viewport", [this, deltaTime]()
                      { this->onDrawViewport(deltaTime); }, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
}

void SpriteRigger::onCleanUp()
{
  this->registry.free<MeshComponent, TransformComponent, ColorComponent>();
}

void SpriteRigger::onDrawViewport(float deltaTime)
{
  for (auto entity : this->registry.entities())
  {
    if (entity->is("Bone"))
    {
      auto [mesh, transform, color] = entity->collect<MeshComponent, TransformComponent, ColorComponent>();
      mesh->render(this->renderer, transform, color);
    }
  }
}
