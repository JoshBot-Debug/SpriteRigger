#pragma once
#include "application.h"

struct CTransform
{
  Vec2 position;
  Vec2 rotation;
  Vec2 scale;

  CTransform() : position(Vec2{0, 0}), rotation(Vec2{0, 0}), scale(Vec2{1, 1}) {}
  CTransform(Vec2 position, Vec2 rotation, Vec2 scale) : position(position), rotation(rotation), scale(scale) {}
};

struct CArmature
{
  /**
   * @id
   * Unique identifier for the armature.
   *
   * @zIndex
   * The zIndex of the armature, used to determine what armature to select when trying to grab.
   */
  int id, zIndex;

  /**
   * Display name of the armature.
   */
  std::string name;

  /**
   * All the bones that belong to this armature.
   */
  std::vector<Entity *> bones;

  CArmature(int id, std::string name) : id(id), zIndex(1), name(name) {}
};

struct CBone
{
  /**
   * @id
   * Unique identifier for the bone.
   *
   * @parent
   * Id of the bone to which this bone is attached to. If not parent, this is the root bone
   *
   * @zIndex
   * The zIndex of the bone, used to determine what bone to select when trying to grab.
   */
  int id, parent, zIndex;

  /**
   * Display name of the bone.
   */
  std::string name;

  /**
   * @position
   * The original position of the bone before any animation.
   * Used to return the bone to it's original position after animation.
   *
   * @size
   * The dimensions of the bone
   */
  Vec2 position, size;

  CBone(int id, std::string name) : id(id), parent(0), zIndex(1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}) {}
  CBone(int id, std::string name, int parent) : id(id), parent(parent), zIndex(1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}) {}
  CBone(int id, std::string name, int parent, Vec2 position) : id(id), parent(0), zIndex(1), name(name), position(position), size(Vec2{20, 100}) {}
};