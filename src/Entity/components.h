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
   * The zIndex of the armature, used to determine what armature to select when trying to grab.
   */
  int zIndex;

  /**
   * Display name of the armature.
   */
  const char *name;

  /**
   * All the bones that belong to this armature.
   */
  std::vector<EntityID> bones;

  CArmature(const char *name) : zIndex(1), name(name) {}
};

struct CBone
{
  /**
   * The zIndex of the bone, used to determine what bone to select when trying to grab.
   */
  int zIndex;

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
  const char *name;

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

  CBone(const char *name, EntityID armature) : zIndex(1), armature(armature), parent(-1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
  CBone(const char *name, EntityID armature, EntityID parent) : zIndex(1), armature(armature), parent(-1), name(name), position(Vec2{0, 0}), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
  CBone(const char *name, EntityID armature, EntityID parent, Vec2 position) : zIndex(1), armature(armature), parent(-1), name(name), position(position), size(Vec2{20, 100}), color(Vec3{255,255,255}) {}
};