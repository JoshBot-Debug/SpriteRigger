#pragma once

#include "imgui.h"

struct Vec2
{
  float x;
  float y;

  Vec2() : x(0), y(0) {}

  Vec2(float x, float y) : x(x), y(y) {}

  Vec2(const ImVec2 &vec) : x(vec.x), y(vec.y) {}

  void set(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  friend bool operator==(const Vec2 &lhs, const Vec2 &rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }

  friend bool operator!=(const Vec2 &lhs, const Vec2 &rhs)
  {
    return !(lhs == rhs);
  }

  operator ImVec2() const
  {
    return ImVec2(x, y);
  }
};

struct Vec4
{
  float x;
  float y;
  float z;
  float w;

  Vec4() : x(0), y(0), z(0), w(0) {}

  Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  Vec4(const ImVec4 &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

  void set(float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  friend bool operator==(const Vec4 &lhs, const Vec4 &rhs)
  {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
  }

  friend bool operator!=(const Vec4 &lhs, const Vec4 &rhs)
  {
    return !(lhs == rhs);
  }

  operator ImVec4() const
  {
    return ImVec4(x, y, z, w);
  }
};