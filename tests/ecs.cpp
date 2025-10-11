#include "ECS2/Registry.h"
#include <gtest/gtest.h>

ECS2::Registry registry;

TEST(Registry, CreateEntity) {

  struct EBone {};
  struct EHierarchy {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto hierarchy_1 = registry.CreateEntity<EHierarchy>();
  auto h1 = hierarchy_1->GetId();
  auto b1 = bone_1->GetId();

  EXPECT_TRUE(b1 == 1);
  EXPECT_TRUE(h1 == 1);

  auto bone_2 = registry.CreateEntity<EBone>();
  auto hierarchy_2 = registry.CreateEntity<EHierarchy>();
  auto h2 = hierarchy_2->GetId();
  auto b2 = bone_2->GetId();

  EXPECT_TRUE(b2 == 2);
  EXPECT_TRUE(h2 == 2);
}

TEST(Registry, GetEntity) {
  struct EBone {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto bone_2 = registry.CreateEntity<EBone>();

  auto b1 = bone_1->GetId();
  auto b2 = bone_2->GetId();

  auto getBone_1 = registry.GetEntity<EBone>(b1);
  auto getBone_2 = registry.GetEntity<EBone>(b2);

  EXPECT_TRUE(b1 == getBone_1->GetId());
  EXPECT_TRUE(b2 == getBone_2->GetId());
}

TEST(Registry, DeleteEntity) {

  struct EBone {};
  struct EHierarchy {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto hierarchy_1 = registry.CreateEntity<EHierarchy>();
  auto bone_2 = registry.CreateEntity<EBone>();
  auto hierarchy_2 = registry.CreateEntity<EHierarchy>();

  auto h1 = hierarchy_1->GetId();
  auto b1 = bone_1->GetId();
  auto h2 = hierarchy_2->GetId();
  auto b2 = bone_2->GetId();

  registry.DestroyEntity<EHierarchy>(h1);
  auto getHierarchy_1 = registry.GetEntity<EHierarchy>(h1);
  auto getHierarchy_2 = registry.GetEntity<EHierarchy>(h2);

  EXPECT_TRUE(getHierarchy_1 == nullptr);
  EXPECT_TRUE(getHierarchy_2 != nullptr);

  auto hierarchy_3 = registry.CreateEntity<EHierarchy>();

  EXPECT_TRUE(hierarchy_3->GetId() == 1);

  auto entities = registry.GetEntities<EHierarchy>();

  EXPECT_TRUE(entities.size() == 2);
}

TEST(Registry, AddComponents) {
  struct EBone {};
  struct CHovered {};
  struct CSelected {};

  auto bone = registry.CreateEntity<EBone>();

  EXPECT_TRUE(bone->Has<CHovered>() == false);
  EXPECT_TRUE(bone->Has<CSelected>() == false);

  bone->Add<CHovered>();
  bone->Add<CSelected>();

  EXPECT_TRUE(bone->Has<CHovered>() == true);
  EXPECT_TRUE(bone->Has<CSelected>() == true);

  auto cHovered = registry.Get<EBone, CHovered>(bone->GetId());
  auto cSelected = registry.Get<EBone, CSelected>(bone->GetId());

  EXPECT_TRUE(cHovered != nullptr);
  EXPECT_TRUE(cSelected != nullptr);

  auto hovered = registry.Get<CHovered>();
  auto selected = registry.Get<CSelected>();

  EXPECT_TRUE(hovered.size() == 1);
  EXPECT_TRUE(selected.size() == 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}