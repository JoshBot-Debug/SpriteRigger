#include "assets.h"

void Assets::importAssets(std::vector<std::string> paths)
{
  for (auto path : paths)
    printf("Paths: %s\n", path.c_str());
}
