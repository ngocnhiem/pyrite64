/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "n64Material.h"

namespace
{
  constexpr uint32_t getBits(uint64_t value, uint32_t start, uint32_t end) {
    return (value << (63 - end)) >> (63 - end + start);
  }
}

void Renderer::N64Material::convert(N64Mesh::MeshPart &part, const Material &t3dMat)
{
  auto &texA = t3dMat.texA;
  auto &texB = t3dMat.texB;

  uint64_t cc = t3dMat.colorCombiner;
  part.material.cc0Color = { getBits(cc, 52, 55), getBits(cc, 28, 31), getBits(cc, 47, 51), getBits(cc, 15, 17) };
  part.material.cc0Alpha = { getBits(cc, 44, 46), getBits(cc, 12, 14), getBits(cc, 41, 43), getBits(cc, 9, 11)  };
  part.material.cc1Color = { getBits(cc, 37, 40), getBits(cc, 24, 27), getBits(cc, 32, 36), getBits(cc, 6, 8)   };
  part.material.cc1Alpha = { getBits(cc, 21, 23), getBits(cc, 3, 5),   getBits(cc, 18, 20), getBits(cc, 0, 2)   };

  part.material.colPrim = {
    t3dMat.primColor[0] / 255.0f,
    t3dMat.primColor[1] / 255.0f,
    t3dMat.primColor[2] / 255.0f,
    t3dMat.primColor[3] / 255.0f
  };
  part.material.colEnv = {
    t3dMat.envColor[0] / 255.0f,
    t3dMat.envColor[1] / 255.0f,
    t3dMat.envColor[2] / 255.0f,
    t3dMat.envColor[3] / 255.0f,
  };
  part.material.mask = {
    texA.s.mask, texA.t.mask,
    texB.s.mask, texB.t.mask,
  };
  part.material.shift = {
    texA.s.shift, texA.t.shift,
    texB.s.shift, texB.t.shift,
  };
  part.material.low = {
    texA.s.low, texA.t.low,
    texB.s.low, texB.t.low,
  };
  part.material.high = {
    texA.s.high, texA.t.high,
    texB.s.high, texB.t.high,
  };

  part.material.geoMode = t3dMat.drawFlags;
  part.material.otherModeH = t3dMat.otherModeValue >> 32;
  part.material.otherModeL = t3dMat.otherModeValue & 0xFFFFFFFF;
}
