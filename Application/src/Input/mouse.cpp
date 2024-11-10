#include "mouse.h"

bool Mouse::grab(GrabId id)
{
  if (this->grabId != -1)
    return false;
  this->grabId = id;
  return true;
}

bool Mouse::release(GrabId id)
{
  if (id != -1 && this->grabId != id)
    return false;
  this->grabId = -1;
  this->grabPayload.reset();
  return true;
}

bool Mouse::isGrabbing(GrabId id)
{
  if (id == -1)
    return this->grabId != -1;
  return this->grabId == id;
}

GrabId Mouse::getGrabId()
{
  return this->grabId;
}