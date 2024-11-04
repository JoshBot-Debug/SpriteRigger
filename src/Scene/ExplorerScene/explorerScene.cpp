#include "explorerScene.h"

void ExplorerScene::onInitialize()
{
  Registry *registry = this->application->getRegistry();

  Entity *bone1 = registry->createEntity("Bone");
  bone1->add<PropertiesComponent>("Bone 1", 1);
  bone1->add<MeshComponent>(Vec2{100, 100});
  bone1->add<TransformComponent>(Vec2{100, 100});
  bone1->add<ColorComponent>(Vec4{255, 255, 255, 255});

  Entity *bone2 = registry->createEntity("Bone");
  bone2->add<PropertiesComponent>("Bone 2", 2);
  bone2->add<MeshComponent>(Vec2{100, 100});
  bone2->add<TransformComponent>(Vec2{100, 300});
  bone2->add<ColorComponent>(Vec4{255, 0, 0, 255});
}

void ExplorerScene::onInput(SDL_Event *event, float deltaTime)
{
}

void ExplorerScene::onUpdate(float deltaTime)
{
}

void ExplorerScene::onDraw(float deltaTime)
{
  Registry *registry = this->application->getRegistry();

  ImGui::Begin("Window");

  for (auto *entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [mesh, properties, transform, color] = entity->collect<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>();
      ImGui::PushID(entity->getId());

      ImGui::SeparatorText(properties->name);
      ImGui::SliderInt("Z index", &properties->zIndex, 0, 10);

      ImGui::SeparatorText("Mesh");
      ImGui::SliderFloat("Width", &mesh->size.x, 0, 200);
      ImGui::SliderFloat("Height", &mesh->size.y, 0, 200);

      ImGui::SeparatorText("Transform");
      ImGui::InputFloat("Position X", &transform->position.x, 50, 100);
      ImGui::InputFloat("Position Y", &transform->position.y, 50, 100);

      float cf[4]{color->color.x / 255.0f, color->color.y / 255.0f, color->color.z / 255.0f, color->color.w / 255.0f};

      ImGui::SeparatorText("Color");
      if (ImGui::ColorPicker4("Background color", cf))
      {
        color->color.x = cf[0] * 255.0f;
        color->color.y = cf[1] * 255.0f;
        color->color.z = cf[2] * 255.0f;
        color->color.w = cf[3] * 255.0f;
      }

      ImGui::PopID();
    }
  }

  ImGui::End();
}

void ExplorerScene::onCleanUp()
{
}
