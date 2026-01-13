/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "nodeEditor.h"

#include "imgui.h"
#include "../../../context.h"
#include "../../../utils/logger.h"
#include "../../imgui/helper.h"

#include "ImNodeFlow.h"

ImFlow::ImNodeFlow mINF;

namespace
{

  class P64Node : public ImFlow::BaseNode
  {
    public:
      virtual std::string serialize() = 0;
      virtual void deserialize(const std::string& data) = 0;
  };

  class SimpleSum : public P64Node {
    public:
      SimpleSum() {
        setTitle("Delay (sec.)");
        setStyle(ImFlow::NodeStyle::green());
        ImFlow::BaseNode::addIN<int>("In", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]() { return getInVal<float>("In") + m_valB; });
      }

      void draw() override {
        //ImGui::SetNextItemWidth(100.f);

        ImGui::InputFloat("##ValB", &m_valB);
        //printf("aa: %f | %08X\n", m_valB, ImGui::GetItemID());
        if(ImGui::Button("aaaa"))m_valB++;
      }

      std::string serialize() override
      {
        return "a";
      }

      void deserialize(const std::string& data) override
      {

      }

    private:
      float m_valB = 0;
  };

  class CollapsingNode : public P64Node {
    public:
      CollapsingNode() {
        setTitle("Logic (AND)");
        setStyle(ImFlow::NodeStyle::red());
        ImFlow::BaseNode::addIN<int>("A", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("B", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addOUT<int>("Out", nullptr)->behaviour([this]() { return getInVal<int>("A") + getInVal<int>("B"); });
      }

      void draw() override {
        /*if(ImFlow::BaseNode::isSelected()) {
          ImGui::SetNextItemWidth(100.f);
          ImGui::Text("You can only see me when the node is selected!");
        }*/
      }

      std::string serialize() override
      {
        return "a";
      }

      void deserialize(const std::string& data) override
      {

      }

  };

  class ResultNode : public P64Node {
    public:
      ResultNode() {
        setTitle("Result node 2");
        setStyle(ImFlow::NodeStyle::brown());
        ImFlow::BaseNode::addIN<int>("A", 0, ImFlow::ConnectionFilter::SameType());
        ImFlow::BaseNode::addIN<int>("B", 0, ImFlow::ConnectionFilter::SameType());
      }

      void draw() override {
        ImGui::Text("Result: %d", getInVal<int>("A") + getInVal<int>("B"));
      }

      std::string serialize() override
      {
        return "a";
      }

      void deserialize(const std::string& data) override
      {

      }
  };


  std::array<std::function<std::shared_ptr<P64Node>(ImFlow::ImNodeFlow &m, const ImVec2&)>, 3> NODE_TABLE = {
    [](ImFlow::ImNodeFlow &m, const ImVec2& pos) { return m.addNode<SimpleSum>(pos); },
    [](ImFlow::ImNodeFlow &m, const ImVec2& pos) { return m.addNode<CollapsingNode>(pos); },
    [](ImFlow::ImNodeFlow &m, const ImVec2& pos) { return m.addNode<ResultNode>(pos); },
  };

  struct SavedNode
  {
    uint32_t type{};
    uint32_t uuid{};
    ImVec2 pos{};
    std::string nodeData{};
  };

  struct SavedNodeLink
  {
    uint32_t uuidNodeA;
    std::string linkA;
    uint32_t uuidNodeB;
    std::string linkB;
  };

}

Editor::NodeEditor::NodeEditor()
{
  std::vector<SavedNode> savedNodes;
  savedNodes.push_back({0, 1, {40, 40}});
  //savedNodes.push_back({0, 2, {40, 150}});
  savedNodes.push_back({2, 3, {250, 80}});

  std::vector<SavedNodeLink> savedLinks;
  //savedLinks.push_back({1, "Out", 3, "A"});
  //savedLinks.push_back({2, "Out", 3, "B"});

  std::unordered_map<uint32_t, std::shared_ptr<P64Node>> newNodes{};
  for(auto &savedNode : savedNodes) {
    auto newNode = NODE_TABLE[savedNode.type](mINF, savedNode.pos);
    newNode->deserialize(savedNode.nodeData);
    newNodes[savedNode.uuid] = newNode;
  }

  for(auto &savedLink : savedLinks) {
    auto nodeAIt = newNodes.find(savedLink.uuidNodeA);
    auto nodeBIt = newNodes.find(savedLink.uuidNodeB);
    if(nodeAIt != newNodes.end() && nodeBIt != newNodes.end()) {
      auto pinA = nodeAIt->second->outPin(savedLink.linkA);
      auto pinB = nodeBIt->second->inPin(savedLink.linkB);
      if(pinA && pinB) {
        pinA->createLink(pinB);
      }
    }
  }

  // Add links between nodes
  //n1->outPin("Out")->createLink(result->inPin("A"));
  //n2->outPin("Out")->createLink(result->inPin("B"));

  // Add a collapsing node
  //auto collapsingNode = mINF.addNode<CollapsingNode>({300, 300});
}

Editor::NodeEditor::~NodeEditor()
{
}

void Editor::NodeEditor::draw()
{
  mINF.setSize(ImGui::GetContentRegionAvail());
  mINF.update();
/*
  printf("Nodes:\n");
  for (const auto& [uid, node] : mINF.getNodes()) {
      printf(" - Node UID: %llu, Title: %s |", node->getUID(), node->getName().c_str());
      printf("pos=(%.1f, %.1f)\n", node->getPos().x, node->getPos().y);
  }
  printf("Links:\n");
  for (const auto& weakLink : mINF.getLinks()) {
      if (auto link = weakLink.lock()) {
          auto leftPin = link->left();
          auto rightPin = link->right();
          if (leftPin && rightPin) {
              auto leftNode = leftPin->getParent();
              auto rightNode = rightPin->getParent();
              printf(" - Link from Node '%llu' Pin '%llu' to Node '%llu' Pin '%llu'\n",
                     leftNode ? leftNode->getUID() : 0,
                     leftPin->getUid(),
                     rightNode ? rightNode->getUID() : 0,
                     rightPin->getUid()
              );
          }
      }
  }*/
}
