/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#pragma once
#include <fgeom.h>

namespace P64::Math
{
  constexpr float SQRT_2_INV = 0.70710678118f;

  constexpr static float s10ToFloat(uint32_t value, float offset, float scale) {
    return (float)value / 1023.0f * scale + offset;
  }

  static fm_quat_t unpackQuat(uint32_t quatPacked)
  {
    constexpr int BITS = 10;
    constexpr int BIT_MASK = (1 << BITS) - 1;

    uint32_t largestIdx = quatPacked >> 30;
    uint32_t idx0 = (largestIdx + 1) & 0b11;
    uint32_t idx1 = (largestIdx + 2) & 0b11;
    uint32_t idx2 = (largestIdx + 3) & 0b11;

    float q0 = s10ToFloat((quatPacked >> (BITS * 2)) & BIT_MASK, -SQRT_2_INV, SQRT_2_INV*2);
    float q1 = s10ToFloat((quatPacked >> (BITS * 1)) & BIT_MASK, -SQRT_2_INV, SQRT_2_INV*2);
    float q2 = s10ToFloat((quatPacked >> (BITS * 0)) & BIT_MASK, -SQRT_2_INV, SQRT_2_INV*2);

    fm_quat_t q{};
    q.v[idx0] = q0;
    q.v[idx1] = q1;
    q.v[idx2] = q2;
    q.v[largestIdx] = sqrtf(1.0f - q0*q0 - q1*q1 - q2*q2);
    return q;
  }
}
