#pragma once

#include "Serializer/Serializer.h"
#include "Window/Layer.h"

#include <string>

class SerializableLayer : public Layer {
public:
  virtual void Save(Serializer &serializer) = 0;
  virtual void Restore(Serializer &serializer) = 0;
};
