/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include "tiny3d/tools/gltf_importer/src/structs.h"
#include "../../renderer/mesh.h"

namespace Utils::Mesh
{
  void t3dmToMesh(const T3DMData &t3dmData, Renderer::Mesh &mesh);
}
