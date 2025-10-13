#include "ECS2/Registry.h"
#include <gtest/gtest.h>

TEST(Registry, CreateEntity) {

  ECS2::Registry registry;

  struct EBone {};
  struct EHierarchy {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto hierarchy_1 = registry.CreateEntity<EHierarchy>();
  auto h1 = hierarchy_1->GetId();
  auto b1 = bone_1->GetId();

  EXPECT_EQ(b1, 1);
  EXPECT_EQ(h1, 1);

  auto bone_2 = registry.CreateEntity<EBone>();
  auto hierarchy_2 = registry.CreateEntity<EHierarchy>();
  auto h2 = hierarchy_2->GetId();
  auto b2 = bone_2->GetId();

  EXPECT_EQ(b2, 2);
  EXPECT_EQ(h2, 2);
}

TEST(Registry, GetEntity) {

  ECS2::Registry registry;

  struct EBone {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto bone_2 = registry.CreateEntity<EBone>();

  auto b1 = bone_1->GetId();
  auto b2 = bone_2->GetId();

  auto getBone_1 = registry.GetEntity<EBone>(b1);
  auto getBone_2 = registry.GetEntity<EBone>(b2);

  EXPECT_EQ(b1, getBone_1->GetId());
  EXPECT_EQ(b2, getBone_2->GetId());
}

TEST(Registry, DeleteEntity) {

  ECS2::Registry registry;

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

  EXPECT_EQ(getHierarchy_1, nullptr);
  EXPECT_TRUE(getHierarchy_2 != nullptr);

  auto hierarchy_3 = registry.CreateEntity<EHierarchy>();

  EXPECT_EQ(hierarchy_3->GetId(), 1);

  auto entities = registry.GetEntities<EHierarchy>();

  EXPECT_EQ(entities.size(), 2);
}

TEST(Registry, AddComponents) {

  ECS2::Registry registry;

  struct EBone {};
  struct CHovered {};
  struct CSelected {};

  auto bone = registry.CreateEntity<EBone>();

  EXPECT_EQ(bone->Has<CHovered>(), false);
  EXPECT_EQ(bone->Has<CSelected>(), false);

  bone->Add<CHovered>();
  bone->Add<CSelected>();

  EXPECT_EQ(bone->Has<CHovered>(), true);
  EXPECT_EQ(bone->Has<CSelected>(), true);

  auto cHovered = registry.Get<EBone, CHovered>(bone->GetId());
  auto cSelected = registry.Get<EBone, CSelected>(bone->GetId());

  EXPECT_TRUE(cHovered != nullptr);
  EXPECT_TRUE(cSelected != nullptr);

  auto hovered = registry.Get<CHovered>();
  auto selected = registry.Get<CSelected>();

  EXPECT_EQ(hovered.size(), 1);
  EXPECT_EQ(selected.size(), 1);
}

TEST(Registry, GetComponents) {

  ECS2::Registry registry;

  struct EBone {};
  struct CHovered {
    int id;
  };
  struct CSelected {
    int id;
  };

  auto bone_1 = registry.CreateEntity<EBone>();
  auto bone_2 = registry.CreateEntity<EBone>();

  bone_1->Add<CHovered>(1);
  bone_1->Add<CSelected>(2);
  bone_2->Add<CHovered>(3);
  bone_2->Add<CSelected>(4);

  auto g1 = registry.Get<EBone, CHovered>(bone_1->GetId())->id;
  auto g2 = registry.Get<EBone, CSelected>(bone_1->GetId())->id;
  auto g3 = registry.Get<EBone, CHovered>(bone_2->GetId())->id;
  auto g4 = registry.Get<EBone, CSelected>(bone_2->GetId())->id;

  EXPECT_EQ(g1, 1);
  EXPECT_EQ(g2, 2);
  EXPECT_EQ(g3, 3);
  EXPECT_EQ(g4, 4);

  auto many = registry.Get<CHovered, CSelected>();

  EXPECT_EQ(many.size(), 2);

  EXPECT_EQ(many[0].first->GetId(), bone_1->GetId());
  EXPECT_EQ(many[1].first->GetId(), bone_2->GetId());

  EXPECT_EQ(std::get<0>(many[0].second)->id, 1);
  EXPECT_EQ(std::get<1>(many[0].second)->id, 2);
  EXPECT_EQ(std::get<0>(many[1].second)->id, 3);
  EXPECT_EQ(std::get<1>(many[1].second)->id, 4);
}

TEST(Entity, Changes) {

  ECS2::Registry registry;

  struct EBone {};

  struct CHovered {
    int id;
  };

  struct CSelected {
    int id;
  };

  auto bone_1 = registry.CreateEntity<EBone>();
  auto bone_2 = registry.CreateEntity<EBone>();

  bone_1->Add<CHovered>(1);
  bone_1->Add<CSelected>(2);
  bone_2->Add<CHovered>(3);
  bone_2->Add<CSelected>(4);

  bone_1->ClearChanged();

  auto a = bone_1->GetChanged<CHovered>();

  EXPECT_EQ(a, nullptr);

  bone_1->MarkChanged<CHovered, CSelected>();

  bool anyChanged = bone_1->AnyChanged<CHovered, CSelected>();
  bool hasChanged = bone_1->HasChanged<CHovered>();
  auto getChanged = bone_1->GetChanged<CHovered>();
  auto collectChanged = bone_1->CollectChanged<CHovered, CSelected>();

  EXPECT_EQ(anyChanged, true);
  EXPECT_EQ(hasChanged, true);
  EXPECT_EQ(getChanged->id, 1);
  EXPECT_EQ(std::get<0>(collectChanged.second)->id, 1);
  EXPECT_EQ(std::get<1>(collectChanged.second)->id, 2);

  bone_1->ClearChanged<CHovered>();
  hasChanged = bone_1->HasChanged<CHovered>();
  EXPECT_EQ(hasChanged, false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}