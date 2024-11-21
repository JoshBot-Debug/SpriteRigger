#include "RenderSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Component/Component.h"
#include "ECS/Entity.h"

void RenderSystem::draw(float deltaTime)
{
  for (const auto &entity : this->registry->entities())
  {
    if (entity->is("Armature"))
    {
      const auto [cShader, cArmature] = entity->collect<CShader, CArmature>();
      cShader->shader->bind();
      cShader->shader->addUniformMatrix4fv("viewProjection", this->camera->getViewProjection());

      for (const auto &boneEID : cArmature->bones)
      {
        const auto [cTransfrom, cBone] = this->registry->collect<CTransform, CBone>(boneEID);
        std::vector<float> instance = {cTransfrom->position.x, cTransfrom->position.y, cBone->color.r, cBone->color.g, cBone->color.b};
        this->resourceManager->updateBone(boneEID, instance);
      }
    }
  }

  this->resourceManager->drawBone();
}

void RenderSystem::setRegistry(Registry *registry)
{
  this->registry = registry;
}

void RenderSystem::setCamera(OrthographicCamera *camera)
{
  this->camera = camera;
}

void RenderSystem::setResourceManager(ResourceManager *resourceManager)
{
  this->resourceManager = resourceManager;
}
