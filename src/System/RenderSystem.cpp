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
  for (auto &entity : this->registry->entities())
  {
    if (entity->is("Armature"))
    {
      const auto [cShader, cMesh] = entity->collect<CShader, CMesh>();
      cShader->shader->bind();
      cShader->shader->addUniformMatrix4fv("viewProjection", this->camera->getViewProjection());
      cMesh->mesh->drawInstances();
    }
  }
}

void RenderSystem::setRegistry(Registry *registry)
{
  this->registry = registry;
}

void RenderSystem::setCamera(OrthographicCamera *camera)
{
  this->camera = camera;
}
