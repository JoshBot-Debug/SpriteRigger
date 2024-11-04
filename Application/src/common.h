#pragma once

#include "imgui.h"
#include <cmath>

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

  bool intersects(const Vec2 &position, const Vec2 &size) const
  {
    return (x >= position.x && x <= position.x + size.x) &&
           (y >= position.y && y <= position.y + size.y);
  }

  static Vec2 lerp(const Vec2 &start, const Vec2 &end, float t)
  {
    return Vec2{
        start.x + t * (end.x - start.x),
        start.y + t * (end.y - start.y)};
  }

  friend Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs)
  {
    return Vec2{
        lhs.x + rhs.x,
        lhs.y + rhs.y};
  }

  friend Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs)
  {
    return Vec2{
        lhs.x - rhs.x,
        lhs.y - rhs.y};
  }

  Vec2 &operator+=(const Vec2 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vec2 &operator-=(const Vec2 &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
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
  float x, y, z, w;

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

  bool intersects(const ImVec4 &position, const ImVec4 &size) const
  {
    return (x >= position.x && x <= position.x + size.x) &&
           (y >= position.y && y <= position.y + size.y) &&
           (z >= position.z && z <= position.z + size.z) &&
           (w >= position.w && w <= position.w + size.w);
  }

  static ImVec4 lerp(const ImVec4 &start, const ImVec4 &end, float t)
  {
    return ImVec4{
        start.x + t * (end.x - start.x),
        start.y + t * (end.y - start.y),
        start.z + t * (end.z - start.z),
        start.w + t * (end.w - start.w),
    };
  }

  friend Vec4 operator+(const Vec4 &lhs, const Vec4 &rhs)
  {
    return Vec4{
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
        lhs.w + rhs.w,
    };
  }

  friend Vec4 operator-(const Vec4 &lhs, const Vec4 &rhs)
  {
    return Vec4{
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z,
        lhs.w - rhs.w,
    };
  }

  Vec4 &operator+=(const Vec4 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
  }

  Vec4 &operator-=(const Vec4 &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
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