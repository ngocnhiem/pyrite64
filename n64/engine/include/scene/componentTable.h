/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <libdragon.h>

namespace P64
{
  class Object;

  typedef uint32_t(*FuncGetAllocSize)(void*);
  typedef void(*FuncInitDel)(Object&, void*, void*);
  typedef void(*FuncUpdate)(Object&, void*);
  typedef void(*FuncDraw)(Object&, void*);

  struct ComponentDef
  {
    FuncInitDel initDel{};
    FuncUpdate update{};
    FuncDraw draw{};
    FuncGetAllocSize getAllocSize{};
  };

  constexpr uint32_t COMP_TABLE_SIZE = 8;
  extern const ComponentDef COMP_TABLE[COMP_TABLE_SIZE];
}