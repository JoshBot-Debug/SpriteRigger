#pragma once

#include <vector>
#include <string>

class Asset
{
private:
public:
  Asset() = default;
  ~Asset() = default;

  void importAssets(std::vector<std::string> paths);
};