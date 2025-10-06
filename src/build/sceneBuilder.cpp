/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "projectBuilder.h"
#include "../utils/string.h"
#include <filesystem>

#include "../utils/binaryFile.h"
#include "../utils/fs.h"

namespace fs = std::filesystem;

namespace
{
  constexpr uint32_t FLAG_CLR_DEPTH = 1 << 0;
  constexpr uint32_t FLAG_CLR_COLOR = 1 << 1;
  constexpr uint32_t FLAG_SCR_32BIT = 1 << 2;
}

void Build::buildScene(Project::Project &project, const Project::SceneEntry &scene, SceneCtx &ctx)
{
  std::string fileNameScene = "s" + Utils::padLeft(std::to_string(scene.id), '0', 4);
  std::string fileNameStr = fileNameScene + "s";

  std::unique_ptr<Project::Scene> sc{new Project::Scene(scene.id)};

  auto fsDataPath = fs::absolute(fs::path{project.getPath()} / "filesystem" / "p64");

  uint32_t flags = 0;
  uint32_t objCount = 0;

  if (sc->conf.doClearDepth)flags |= FLAG_CLR_DEPTH;
  if (sc->conf.doClearColor)flags |= FLAG_CLR_COLOR;
  if (sc->conf.fbFormat)flags |= FLAG_SCR_32BIT;

  ctx.fileScene = {};
  ctx.fileScene.write<uint16_t>(sc->conf.fbWidth);
  ctx.fileScene.write<uint16_t>(sc->conf.fbHeight);
  ctx.fileScene.write(flags);
  ctx.fileScene.writeRGBA(sc->conf.clearColor);
  ctx.fileScene.write(objCount);

  ctx.fileScene.writeToFile(fsDataPath / fileNameScene);

  Utils::FS::saveTextFile(fsDataPath / fileNameStr, "TODO");

  ctx.files.push_back("filesystem/p64/" + fileNameScene);
  ctx.files.push_back("filesystem/p64/" + fileNameStr);
}
