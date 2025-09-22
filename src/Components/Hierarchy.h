#pragma once

#include <imgui.h>

#include <functional>
#include <stdint.h>
#include <unordered_map>
#include <vector>

class Hierarchy {
public:
  using NodeId = uint32_t;

  struct Item {
    NodeId id;
    NodeId parent;
    std::string label;
  };

private:
  static const ImGuiTreeNodeFlags s_TreeNodeBaseFlags =
      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen |
      ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_FramePadding;

  static const ImGuiTableFlags s_TableFlags =
      ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
      ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
      ImGuiTableFlags_NoBordersInBody;

  struct Node {
    Item item;
    std::vector<Node *> children = {};

    void Add(const Item &item) {
      Node *node = Find(item.parent, this);
      if (node == nullptr)
        throw std::runtime_error("Invalid parent id: " +
                                 std::to_string(item.parent));
      node->children.emplace_back(new Node{.item = item});
    }

    Node *Find(NodeId id, Node *node) {
      if (node == nullptr)
        return nullptr;
      if (node->item.id == id)
        return node;
      for (auto *child : node->children) {
        Node *result = Find(id, child);
        if (result)
          return result;
      }
      return nullptr;
    };

    void Render(const std::function<void(Item *item)> &renderItem) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      bool hasChildren = children.size() > 0;

      ImGuiTreeNodeFlags treeFlags = s_TreeNodeBaseFlags;

      if (!hasChildren)
        treeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                     ImGuiTreeNodeFlags_NoTreePushOnOpen;

      bool opened = ImGui::TreeNodeEx(item.label.c_str(), treeFlags);

      renderItem(&item);

      if (hasChildren && opened) {
        for (auto *child : children)
          child->Render(renderItem);

        ImGui::TreePop();
      }
    }
  };

private:
  Node *m_Root = new Node();
  std::function<void(Item *item)> m_RenderItem = nullptr;

public:
  Hierarchy() = default;

  void Add(const Item &item) { m_Root->Add(item); }

  void Remove(NodeId id){};

  void OnRenderItem(const std::function<void(Item *item)> &callback) {
    m_RenderItem = callback;
  }

  void Render(const std::string &id) {
    if (m_Root->children.size() == 0)
      return;

    if (ImGui::BeginTable(id.c_str(), 1, s_TableFlags)) {

      for (auto *child : m_Root->children)
        child->Render(m_RenderItem);

      ImGui::EndTable();
    }
  }
};