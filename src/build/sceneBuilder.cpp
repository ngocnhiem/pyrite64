/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "projectBuilder.h"
#include "../utils/string.h"
#include <filesystem>

namespace fs = std::filesystem;

void Build::buildScene(Project::Project &project, const Project::SceneEntry &scene)
{
  printf(" - Scene %d: %s\n", scene.id, scene.name.c_str());
  std::string fileName = "s" + Utils::padLeft(std::to_string(scene.id), '0', 4);

  auto fsDataPath = fs::absolute(fs::path{project.getPath()} / "filesystem" / "p64");
  auto sceneFile = fsDataPath / fileName;

}
