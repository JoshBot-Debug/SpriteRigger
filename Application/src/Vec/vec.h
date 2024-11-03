#pragma once

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
