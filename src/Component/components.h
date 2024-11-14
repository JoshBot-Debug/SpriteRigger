#pragma once
#include "application.h"

struct CTransform
{
  glm::vec2 position;
  glm::vec2 rotation;
  glm::vec2 scale;

  CTransform() : position(glm::vec2{0, 0}), rotation(glm::vec2{0, 0}), scale(glm::vec2{1, 1}) {}
  CTransform(glm::vec2 position, glm::vec2 rotation, glm::vec2 scale) : position(position), rotation(rotation), scale(scale) {}
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
   * The size of the bone, this is basically the collision box.
   * The mesh may not intersect with the mouse but if the mouse is in
   * the collision box, we know we are making contact.
   * Checking the verticies to calculate if we are colliding is not possible since
   * the data is stored in a buffer on the GPU. We do not want to store that data on the cpu.
   */
  glm::vec2 position, size;

  /**
   * The color of the bone
   */
  glm::vec3 color;

  CBone(std::string name, EntityID armature) : armature(armature), parent(-1), name(name), position(glm::vec2{0, 0}), size(glm::vec2{20, 100}), color(glm::vec3{255, 255, 255}) {}
  CBone(std::string name, EntityID armature, EntityID parent) : armature(armature), parent(-1), name(name), position(glm::vec2{0, 0}), size(glm::vec2{20, 100}), color(glm::vec3{255, 255, 255}) {}
  CBone(std::string name, EntityID armature, EntityID parent, glm::vec2 position) : armature(armature), parent(-1), name(name), position(position), size(glm::vec2{20, 100}), color(glm::vec3{255, 255, 255}) {}
};