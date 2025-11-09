#pragma once

#include <imgui.h>

#include <iostream>
#include <stdint.h>
#include <unordered_map>
#include <vector>

class Hierarchy {
public:
  using NodeId = uint32_t;

  struct Item {
    NodeId id = 0;
    NodeId parent = 0;
    std::string label;
    ImGuiTreeNodeFlags flags;
    void (*onRenderItem)(Item *item) = nullptr;
  };

private:
  static const ImGuiTreeNodeFlags s_TreeNodeBaseFlags =
      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen |
      ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_FramePadding;

  static const ImGuiTableFlags s_TableFlags =
      ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
      ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX |
      ImGuiTableFlags_NoPadOuterX;

  struct Node {
    Item item;
    std::vector<Node *> children = {};

    ~Node() {
      for (auto *child : children)
        delete child;
      children.clear();
    }

    void Add(const Item &item) {
      if (Find(item.id, this))
        return;
      Node *node = Find(item.parent, this);
      if (node == nullptr) {
        std::cerr << "Invalid parent id:" << std::to_string(item.parent)
                  << "\n";
        return;
      }
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

    void Render(void (*renderItem)(Item *, void *), void *data) {
      ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      bool hasChildren = children.size() > 0;

      ImGuiTreeNodeFlags treeFlags = s_TreeNodeBaseFlags | item.flags;

      if (!hasChildren)
        treeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet |
                     ImGuiTreeNodeFlags_NoTreePushOnOpen;

      std::string label = (item.label.size() == 0
                               ? ("##HierarchyItem:" + std::to_string(item.id))
                               : label);
      bool opened = ImGui::TreeNodeEx(label.c_str(), treeFlags);

      if (item.onRenderItem)
        item.onRenderItem(&item);
      else
        renderItem(&item, data);

      if (hasChildren && opened) {
        for (auto *child : children)
          child->Render(renderItem, data);

        ImGui::TreePop();
      }

      ImGui::PopStyleVar();
    }
  };

private:
  Node *m_Root = new Node();
  void *m_RenderItemData = nullptr;
  void (*m_RenderItem)(Item *, void *) = nullptr;

public:
  Hierarchy() = default;
  ~Hierarchy() { delete m_Root; }

  void Add(const Item &item) { m_Root->Add(item); }

  void Remove(NodeId id) { delete m_Root->Find(id, m_Root); };

  void Clear() {
    for (auto *child : m_Root->children)
      delete child;
    m_Root->children.clear();
  };

  void OnRenderItem(void (*callback)(Item *, void *)) {
    m_RenderItem = callback;
  }

  void SetRenderItemData(void *renderItemData) {
    m_RenderItemData = renderItemData;
  }

  void Render(const std::string &id, void *renderItemData = nullptr) {
    if (renderItemData)
      m_RenderItemData = renderItemData;

    if (m_Root->children.size() == 0)
      return;

    if (ImGui::BeginTable(id.c_str(), 1, s_TableFlags)) {
      for (auto *child : m_Root->children)
        child->Render(m_RenderItem, m_RenderItemData);

      ImGui::EndTable();
    }
  }
};