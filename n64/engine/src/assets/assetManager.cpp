/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "assets/assetManager.h"

#include <libdragon.h>

#include "assets/assetTypes.h"
#include "lib/logger.h"
#include "scene/components/model.h"

namespace
{
  struct AssetEntry
  {
    const char* path{};
    void* data{};

    uint32_t getType() const {
      return (uint32_t)data >> 24;
    }

    void* getPointer() const {
      return (void*)(
        ((uint32_t)data & 0x00FF'FFFF)
      );
    }

    void setPointer(void* ptr) {
      uint32_t ptrMasked = (uint32_t)ptr & 0x00FF'FFFF;
      uint32_t typeMasked = (uint32_t)data & 0xFF00'0000;
      data = (void*)(ptrMasked | typeMasked);
    }
  };

  typedef void* (*LoadFunc)(const char* path);
  typedef void (*FreeFunc)(void* ref);

  struct AssetTable
  {
    uint32_t count{};
    AssetEntry entries[];
  };

  void* load_t3dm(const char* path) {
    auto model = t3d_model_load(path);
    rspq_block_begin();
      t3d_model_draw(model);
    model->userBlock = rspq_block_end();
    return model;
  }

  struct AssetHandler
  {
    LoadFunc fnLoad{};
    FreeFunc fnFree{};
  };

  AssetHandler assetHandler[] = {
    {nullptr, nullptr},                    // UNKNOWN
    {(LoadFunc)sprite_load, (FreeFunc)sprite_free}, // IMAGE
    {nullptr, nullptr},                    // AUDIO
    {(LoadFunc)load_t3dm, (FreeFunc)t3d_model_free},
    {nullptr, nullptr},                    // CODE
    {nullptr, nullptr},                    // PREFAB
  };

  constinit AssetTable* assetTable{nullptr};
  constinit bool isInit{false};
}

void P64::AssetManager::init() {
  if (isInit)return;
  isInit = true;

  assetTable = (AssetTable*)asset_load("rom:/p64/a", nullptr);
  for (uint32_t i = 0; i < assetTable->count; ++i) {
    auto &entry = assetTable->entries[i];
    uint32_t offset = (uint32_t)entry.path;
    entry.path = (char*)assetTable + offset;
  }
}

void P64::AssetManager::freeAll() {
  Log::warn("TODO: P64::AssetManager::freeAll");
}

void* P64::AssetManager::getByIndex(uint32_t idx) {
  if (idx >= assetTable->count) {
    return nullptr;
  }

  auto &entry = assetTable->entries[idx];

  void* res = entry.getPointer();
  if (!res) {
    auto type = entry.getType();
    const auto &loader = assetHandler[type];
    assertf(loader.fnLoad != nullptr, "No asset loader for type: %lu", type);
    res = loader.fnLoad(entry.path);
    entry.setPointer(res);
    debugf("Load Asset: %s | %lu\n", entry.path, type);
  } else {
    res = (void*)((uint32_t)res | 0x8000'0000);
  }

  return res;

}