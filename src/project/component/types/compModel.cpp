/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "../components.h"
#include "../../../context.h"
#include "../../../editor/imgui/helper.h"
#include "../../../utils/json.h"
#include "../../../utils/jsonBuilder.h"
#include "../../../utils/binaryFile.h"
#include "../../../utils/logger.h"
#include "../../assetManager.h"

namespace Project::Component::Model
{
  struct Data
  {
    uint64_t modelUUID{0};
    Renderer::Object obj3D{};
    bool obj3DLoaded{false};
  };

  std::shared_ptr<void> init(Object &obj) {
    auto data = std::make_shared<Data>();
    return data;
  }

  std::string serialize(Entry &entry) {
    Data &data = *static_cast<Data*>(entry.data.get());
    Utils::JSON::Builder builder{};
    builder.set("model", data.modelUUID);
    return builder.toString();
  }

  std::shared_ptr<void> deserialize(simdjson::simdjson_result<simdjson::dom::object> &doc) {
    auto data = std::make_shared<Data>();
    data->modelUUID = doc["model"].get<uint64_t>();
    return data;
  }

  void build(Object&, Entry &entry, Build::SceneCtx &ctx)
  {
    Data &data = *static_cast<Data*>(entry.data.get());

    /*auto idRes = ctx.codeIdxMapUUID.find(data.scriptUUID);
    uint16_t id = 0xDEAD;
    if (idRes == ctx.codeIdxMapUUID.end()) {
      Utils::Logger::log("Component Code: Script UUID not found: " + std::to_string(entry.uuid), Utils::Logger::LEVEL_ERROR);
    } else {
      id = idRes->second;
    }

    ctx.fileObj.write<uint16_t>(id);
    ctx.fileObj.write<uint16_t>(0);

    auto script = ctx.project->getAssets().getEntryByUUID(data.scriptUUID);
    if (!script)return;

    for (auto &field : script->params.fields) {
      auto val = data.args[field.name];
      if (val.empty())val = field.defaultValue;
      if (val.empty())val = "0";
      ctx.fileObj.writeAs(val, field.type);
    }
    */
  }

  const char* getter(void* user_data, int idx)
  {
    auto &scriptList = ctx.project->getAssets().getTypeEntries(AssetManager::FileType::MODEL_3D);
    if (idx < 0 || idx >= scriptList.size())return "<Select Model>";
    return scriptList[idx].name.c_str();
  }

  void draw(Object &obj, Entry &entry)
  {
    Data &data = *static_cast<Data*>(entry.data.get());

    auto &assets = ctx.project->getAssets();
    auto &modelList = assets.getTypeEntries(AssetManager::FileType::MODEL_3D);

    if (ImGui::InpTable::start("Comp")) {
      ImGui::InpTable::addString("Name", entry.name);
      ImGui::InpTable::add("Model");
      //ImGui::InputScalar("##UUID", ImGuiDataType_U64, &data.scriptUUID);

      int idx = modelList.size();
      for (int i=0; i<modelList.size(); ++i) {
        if (modelList[i].uuid == data.modelUUID) {
          idx = i;
          break;
        }
      }

      if (ImGui::Combo("##UUID", &idx, getter, nullptr, modelList.size()+1)) {
        data.obj3DLoaded = false;
      }

      if (idx < modelList.size()) {
        const auto &script = modelList[idx];
        data.modelUUID = script.uuid;
      }

      ImGui::InpTable::end();
    }
  }

  void draw3D(Object& obj, Entry &entry, SDL_GPUCommandBuffer* cmdBuff, SDL_GPURenderPass* pass)
  {
    Data &data = *static_cast<Data*>(entry.data.get());
    if (!data.obj3DLoaded) {
      auto asset = ctx.project->getAssets().getEntryByUUID(data.modelUUID);
      if (asset && asset->mesh3D) {
        data.obj3D.setMesh(asset->mesh3D);
      }
      data.obj3DLoaded = true;
    }

    data.obj3D.setPos(obj.pos);
    data.obj3D.draw(pass, cmdBuff);
  }
}
