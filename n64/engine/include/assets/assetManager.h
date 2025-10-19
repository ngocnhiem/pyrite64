/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>

namespace P64::AssetManager
{
  void init();
  void freeAll();

  void* getByIndex(uint32_t idx);
}