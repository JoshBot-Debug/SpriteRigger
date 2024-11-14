#pragma once

#include <stdio.h>
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