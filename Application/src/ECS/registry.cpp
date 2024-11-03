#include "entity.h"
#include "registry.h"

Registry::~Registry()
{
  for (auto entity : this->entities)
    delete entity;
}

Entity *Registry::createEntity()
{
  ++nEID;
  Entity *entity = new Entity(this->nEID, this);
  this->entities.push_back(entity);
  return entity;
}