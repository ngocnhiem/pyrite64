/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include "../project/project.h"

namespace Build
{
  void buildScene(Project::Project &project, const Project::SceneEntry &scene);
  bool buildProject(Project::Project &project);
}
