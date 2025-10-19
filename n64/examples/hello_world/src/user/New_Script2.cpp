#include "scene/sceneManager.h"

namespace P64::Script::C3AF5D870988CBC0
{
  struct Data
  {
    [[P64::Name("Scene ID")]]
    uint32_t sceneId;

    [[P64::Name("Speed"), P64::Min("1")]]
    float speed = 1.0f;
  };

  void update(Object& obj, Data *data)
  {
    auto pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if (pressed.b) {
      debugf("Arg: Scene-Id: %ld\n", data->sceneId);
      SceneManager::load(data->sceneId);
    }

    data->speed += 0.1f;
  }

  void draw(Object& obj, Data *data)
  {
    uint8_t green = (fm_sinf(data->speed) * 0.5f + 0.5f) * 255;
    rdpq_set_prim_color({0xFF, green, 0, 0xFF});
  }
}
