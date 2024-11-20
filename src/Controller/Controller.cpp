#include "Controller.h"
#include "Render/Mesh/Mesh.h"
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
  /**
   * Temp creating the shader here, in general, it would
   * have been created in the asset manager and linked to this 
   * CShader component through the UI
   * The shader will be managed through a ShaderManager / ResourceManager
   */
  Shader *shader = new Shader();
  shader->compile("src/Shader/vertex.glsl", GL_VERTEX_SHADER);
  shader->compile("src/Shader/fragment.glsl", GL_FRAGMENT_SHADER);
  shader->createProgram();

  Mesh *mesh = this->resourceManager->createBone();
  this->resourceManager->createBoneInstance(mesh);

  Entity *armature = this->registry->createEntity("Armature");
  CArmature *cArmature = armature->add<CArmature>("Armature 1");
  armature->add<CMesh>(mesh);
  armature->add<CShader>(shader);

  Entity *bone = this->registry->createEntity("Bone");
  bone->add<CTransform>();
  bone->add<CBone>("Bone 1", *armature);

  cArmature->bones.push_back(*bone);
}
