#include "Controller.h"
#include "Render/Model/InstancedMesh.h"
#include "Render/Shader/Shader.h"
#include <iostream>

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::onInitialize(Registry *registry, ResourceManager *resourceManager)
{
  this->registry = registry;
  this->resourceManager = resourceManager;
}

void Controller::createArmature()
{
  ++armatureCount;

  /**
   * Temp creating the shader here, in general, it would
   * have been created in the asset manager and linked to this
   * CShader component through the UI
   * The shader will be managed through a ShaderManager / ResourceManager
   */
  Shader *shader = new Shader();
  shader->compile("src/Shader/vertex.glsl", ShaderType::VERTEX_SHADER);
  shader->compile("src/Shader/fragment.glsl", ShaderType::FRAGMENT_SHADER);
  shader->createProgram();

  Entity *armature = this->registry->createEntity("Armature");
  std::string armatureName = "Armature " + std::to_string(armature->getId());
  CArmature *cArmature = armature->add<CArmature>(armatureName);
  armature->add<CShader>(shader);

  Entity *bone = this->registry->createEntity("Bone");
  CTransform *cTransform = bone->add<CTransform>();
  cTransform->scale = {100.0f, 100.0f};

  std::string boneName = "Bone " + std::to_string(bone->getId());
  CBone *cBone = bone->add<CBone>(boneName, *armature);
  cBone->size = {100.0f, 100.0f};
  cBone->color = {1.0f, 0.0f, 0.0f};

  this->resourceManager->addBone(bone->getId());

  cArmature->bones.push_back(*bone);
}
