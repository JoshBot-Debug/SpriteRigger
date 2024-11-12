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
   * Display name of the armature.
   */
  std::string name;

  /**
   * All the bones that belong to this armature.
   */
  std::vector<EntityID> bones;

  CArmature(std::string name) : name(name) {}
};

struct CBone
{
  /**
   * If the bone is focused
   */
  bool focused;

  /**
   * The entity id of the armature and parent
   * If the parent is -1, it's the root bone
   */
  EntityID armature, parent;

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

  /**
   * The color of the bone
   */
  Vec3 color;

  CBone(std::string name, EntityID armature) : armature(armature), parent(-1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
  CBone(std::string name, EntityID armature, EntityID parent) : armature(armature), parent(-1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
  CBone(std::string name, EntityID armature, EntityID parent, Vec2 position) : armature(armature), parent(-1), name(name), position(position), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
};