#pragma once
#include "application.h"

struct GrabPayload
{
  Vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(Vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

struct ColorComponent
{
  Vec4 color{255, 255, 255, 255};

  ColorComponent(Vec4 color) : color(color) {}
};

struct TransformComponent
{
  Vec2 position;
  Vec2 rotation;

  TransformComponent() : position(Vec2{0, 0}), rotation(Vec2{0, 0}) {}
  TransformComponent(Vec2 position) : position(position), rotation(Vec2{0, 0}) {}
  TransformComponent(Vec2 position, Vec2 rotation) : position(position), rotation(rotation) {}
};

struct MeshComponent
{
  Vec2 size;

  MeshComponent(Vec2 size) : size(size) {}
};

struct PropertiesComponent
{
  const char *name;
  int zIndex;

  PropertiesComponent(const char *name, int zIndex) : name(name), zIndex(zIndex) {}
};