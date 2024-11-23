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
  std::vector<float> transforms;
  std::vector<float> scales;
  std::vector<float> colors;

  transforms.reserve(this->registry->entities().size());
  scales.reserve(this->registry->entities().size());
  colors.reserve(this->registry->entities().size());

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
        transforms.push_back(cTransfrom->position.x);
        transforms.push_back(cTransfrom->position.y);
        transforms.push_back(cTransfrom->rotation);
        scales.push_back(cTransfrom->scale.x);
        scales.push_back(cTransfrom->scale.y);
        colors.push_back(cBone->color.r);
        colors.push_back(cBone->color.g);
        colors.push_back(cBone->color.b);
      }
    }
  }

  this->resourceManager->updateBoneByOffset(0, 0, transforms);
  this->resourceManager->updateBoneByOffset(1, 0, scales);
  this->resourceManager->updateBoneByOffset(2, 0, colors);
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
