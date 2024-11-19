#include "Controller.h"
#include "Render/Mesh/Mesh.h"
#include <iostream>

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::onInitialize(Registry *registry, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem)
{
  this->registry = registry;
  this->colorSystem = colorSystem;
  this->renderSystem = renderSystem;
  this->transformSystem = transformSystem;
}

void Controller::createArmature()
{
  Entity *armature = this->registry->createEntity("Armature");
  CArmature *cArmature = armature->add<CArmature>("Armature 1");

  Entity *bone = this->registry->createEntity("Bone");
  bone->add<CTransform>();
  bone->add<CBone>("Bone 1", *armature);

  cArmature->bones.push_back(*bone);

  Mesh *mesh = new Mesh("Armature");
  mesh->createVertexArray();
      
  std::vector<float> vertices = {
    -50.0f, -50.0f,
     50.0f, -50.0f,
     50.0f,  50.0f,
    -50.0f,  50.0f,
  };
  mesh->createArrayBuffer(vertices);

  std::vector<unsigned int> indices = {
    0, 1, 2,
    0, 2, 3
  };
  
  mesh->createElementArrayBuffer(indices);
  mesh->setVertexAttrib(0, 2, 0);
  mesh->createInstanceBuffer("transform");
  mesh->setInstanceVertexAttrib(1, 3, 0);
  // mesh->createInstanceBuffer("color");
  // mesh->setInstanceVertexAttrib(1, 3, 0);
  mesh->unbind();

  Shader *shader = mesh->createShader();
  shader->compile("src/Shader/vertex.glsl", GL_VERTEX_SHADER);
  shader->compile("src/Shader/fragment.glsl", GL_FRAGMENT_SHADER);
  shader->createProgram();

  std::vector<float> transform = { 1.0f, 0.0f, 0.0f };
  mesh->setInstanceBuffer("transform", transform);
  // std::vector<float> color = { 1.0f, 0.0f, 0.0f };
  // mesh->setInstanceBuffer("color", color);

  this->renderSystem->push(mesh);
}
