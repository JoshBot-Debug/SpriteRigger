#pragma once

#include <vector>
#include <string>

class AssetManager
{
private:
public:
  AssetManager() = default;
  ~AssetManager() = default;

  void importAssets(std::vector<std::string> paths);
};