#include "spriteRigger.h"

#include <stdio.h>
#include <vector>

struct ColorComponent
{
  Vec4 color{255, 255, 255, 255};

  void renderProperties()
  {
    ImGui::SeparatorText("Color");
    ImGui::SliderFloat("Color X", &this->color.x, 0, 255);
    ImGui::SliderFloat("Color Y", &this->color.y, 0, 255);
    ImGui::SliderFloat("Color Z", &this->color.z, 0, 255);
    ImGui::SliderFloat("Color A", &this->color.w, 0, 255);
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
  this->mouse.set(event->button.x, event->button.y);
}

void SpriteRigger::onUpdate(float deltaTime)
{
  auto [x, y] = this->viewport.getMousePosition(this->mouse);

  printf("Viewport mPos: x:%f y:%f\n", x, y);
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->mainMenu.onDraw();

  ImGui::Begin("Window");

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

  this->viewport.draw("Viewport", [this, deltaTime]()
                      { this->onDrawViewport(deltaTime); });
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
