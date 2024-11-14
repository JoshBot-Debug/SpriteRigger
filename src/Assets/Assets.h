#pragma once

#include <stdio.h>
#include <vector>
#include <string>

class Assets
{
private:
public:
  Assets() = default;
  ~Assets() = default;

  void importAssets(std::vector<std::string> paths);
};