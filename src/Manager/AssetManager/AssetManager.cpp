#include "AssetManager.h"

void AssetManager::importAssets(std::vector<std::string> paths)
{
  for (auto path : paths)
    printf("Paths: %s\n", path.c_str());
}
