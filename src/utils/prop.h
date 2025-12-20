/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <cstdint>
#include <utility>

#include "hash.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/quaternion.hpp"


struct GenericValue
{
  std::string valString{};
  union
  {
    glm::quat valQuat{};
    glm::vec3 valVec3;
    glm::vec4 valVec4;
    uint64_t valU64;
    uint32_t valU32;
    int64_t valS64;
    int32_t valS32;
    float valFloat;
    bool valBool;
  };

  template<typename T>
  constexpr T& get()
  {
    if constexpr (std::is_same_v<T, glm::quat>) {
      return valQuat;
    } else if constexpr (std::is_same_v<T, glm::vec3>) {
      return valVec3;
    } else if constexpr (std::is_same_v<T, glm::vec4>) {
      return valVec4;
    } else if constexpr (std::is_same_v<T, uint64_t>) {
      return valU64;
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      return valU32;
    } else if constexpr (std::is_same_v<T, int64_t>) {
      return valS64;
    } else if constexpr (std::is_same_v<T, int32_t>) {
      return valS32;
    } else if constexpr (std::is_same_v<T, float>) {
      return valFloat;
    } else if constexpr (std::is_same_v<T, bool>) {
      return valBool;
    } else if constexpr (std::is_same_v<T, std::string>) {
      return valString;
    } else  {
      static_assert(false, "Unsupported type in GenericValue::get");
    }
  }
};

template<typename T>
struct Property
{
  std::string name{};
  uint64_t id{};
  T value{};

  constexpr Property() = default;

  constexpr explicit Property(const char* const propName)
    : name{propName}, id{Utils::Hash::crc64(name)}
  {}

  constexpr explicit Property(std::string propName, T val)
    : name{std::move(propName)}, id{Utils::Hash::crc64(name)}, value{val}
  {}

  T& resolve(std::unordered_map<uint64_t, GenericValue> &overrides, bool *isOverride = nullptr)
  {
    auto it = overrides.find(id);
    if(it != overrides.end()) {
      if(isOverride)*isOverride = true;
      return it->second.get<T>();
    }
    if(isOverride)*isOverride = false;
    return value;
  }
};

using PropU32 = Property<uint32_t>;
using PropS32 = Property<int32_t>;
using PropU64 = Property<uint64_t>;
using PropS64 = Property<int64_t>;

using PropFloat = Property<float>;
using PropBool = Property<bool>;

using PropVec3 = Property<glm::vec3>;
using PropVec4 = Property<glm::vec4>;
using PropQuat = Property<glm::quat>;

using PropString = Property<std::string>;

#define PROP_U32(name) Property<uint32_t> name{#name}
#define PROP_S32(name) Property<int32_t> name{#name}
#define PROP_U64(name) Property<uint64_t> name{#name}
#define PROP_S64(name) Property<int64_t> name{#name}
#define PROP_FLOAT(name) Property<float> name{#name}
#define PROP_BOOL(name) Property<bool> name{#name}
#define PROP_VEC3(name) Property<glm::vec3> name{#name}
#define PROP_VEC4(name) Property<glm::vec4> name{#name}
#define PROP_QUAT(name) Property<glm::quat> name{#name}
#define PROP_STRING(name) Property<std::string> name{#name}
