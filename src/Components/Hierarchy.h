#pragma once

#include <imgui.h>

#include <functional>
#include <stdint.h>
#include <unordered_map>
#include <vector>

class Hierarchy {
private:
  static const ImGuiTreeNodeFlags s_TreeNodeBaseFlags =
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen |
      ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_FramePadding;

  static const ImGuiTableFlags s_TableFlags =
      ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
      ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
      ImGuiTableFlags_NoBordersInBody;

public:
  using NodeId = uint32_t;

  struct Item {
    NodeId id;
    std::string label;
    std::function<void()> render = nullptr;
  };

private:
  struct Node {
    NodeId id = 0;
    std::string label;
    std::function<void()> render = nullptr;
    std::vector<Node *> children = {};

    void Add(NodeId parent, const Item &item) {
      Node *node = Find(parent, this);
      if (node == nullptr)
        throw std::runtime_error("Invalid parent id: " +
                                 std::to_string(parent));
      node->children.emplace_back(new Node{
          .id = item.id,
          .label = item.label,
          .render = item.render,
      });
    }

    Node *Find(NodeId id, Node *node) {
      if (node == nullptr)
        return nullptr;
      if (node->id == id)
        return node;
      for (auto *child : node->children) {
        Node *result = Find(id, child);
        if (result)
          return result;
      }
      return nullptr;
    };

    static void Render(Node *node) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      if (node->children.size() > 0) {
        if (ImGui::TreeNodeEx(node->label.c_str(), s_TreeNodeBaseFlags)) {
          if (node->render)
            node->render();
          for (auto *child : node->children)
            Render(child);
          ImGui::TreePop();
        }
      } else {
        ImGui::TreeNodeEx(node->label.c_str(),
                          s_TreeNodeBaseFlags | ImGuiTreeNodeFlags_Leaf |
                              ImGuiTreeNodeFlags_Bullet |
                              ImGuiTreeNodeFlags_NoTreePushOnOpen);
        if (node->render)
          node->render();
      }
    }
  };

private:
  Node *m_Root = new Node();

public:
  Hierarchy() = default;

  void Add(NodeId parent, const Item &item) { m_Root->Add(parent, item); }

  void Render(const std::string &id) {

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{0.0f, 0.0f});
    if (ImGui::BeginTable(id.c_str(), 1, s_TableFlags)) {

      for (auto *child : m_Root->children)
        Node::Render(child);

      ImGui::EndTable();
    }
    ImGui::PopStyleVar(1);
  }
};