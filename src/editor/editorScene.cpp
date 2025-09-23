/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "editorScene.h"
#include "imgui.h"

namespace
{
  bool show_demo_window{};
  bool show_another_window{};
  ImVec4 clear_color{0.45f, 0.55f, 0.60f, 1.00f};
  int counter{0};
}

void Editor::Scene::draw()
{
  auto &io = ImGui::GetIO();

  ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

  ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
  ImGui::Checkbox("Another Window", &show_another_window);

  //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
  ImGui::ColorEdit3("Clear-color", (float*)&clear_color, ImGuiColorEditFlags_NoInputs); // Edit 3 floats representing a color

  if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    counter++;
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::End();
}
