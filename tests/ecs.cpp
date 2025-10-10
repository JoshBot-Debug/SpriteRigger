#include "ECS2/Entity.h"
#include <gtest/gtest.h>

ECS2::Registry registry;

TEST(Registry, CreateEntity) {

  struct EBone {};

  auto bone_1 = registry.CreateEntity<EBone>();
  auto bone_2 = registry.CreateEntity<EBone>();

  // GTEST_LOG_(INFO) << "bone_1 = " << bone_1->GetId();
  // GTEST_LOG_(INFO) << "bone_2 = " << bone_2->GetId() << std::endl;

  EXPECT_TRUE(bone_1 < bone_2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}