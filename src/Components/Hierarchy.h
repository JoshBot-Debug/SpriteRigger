#pragma once

#include <imgui.h>

#include <stdint.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class Hierarchy
{
public:
  using NodeId = uint64_t;

  struct Item
  {
    NodeId             id     = 0;
    NodeId             parent = 0;
    std::string        label;
    ImGuiTreeNodeFlags flags;
    void (*onRenderItem)(Item* item) = nullptr;
  };

private:
  static const ImGuiTreeNodeFlags s_TreeNodeBaseFlags =
      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull |
      ImGuiTreeNodeFlags_FramePadding;

  static const ImGuiTableFlags s_TableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                              ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX |
                                              ImGuiTableFlags_NoPadOuterX;

  struct Node
  {
    Item                               item;
    std::vector<std::unique_ptr<Node>> children = {};

    void Add(const Item& item)
    {
      if (Find(item.id))
        return;
      Node* node = Find(item.parent);
      if (node == nullptr)
      {
        std::cerr << "Invalid parent id:" << std::to_string(item.parent) << "\n";
        return;
      }

      node->children.emplace_back(std::make_unique<Node>());
      node->children.back()->item = item;
    }

    bool Remove(NodeId id)
    {
      for (auto it = children.begin(); it != children.end(); ++it)
        if ((*it)->item.id == id)
        {
          children.erase(it);
          return true;
        }

      for (auto& child : children)
        if (child->Remove(id))
          return true;

      return false;
    }

    Node* FindParent(NodeId id)
    {
      for (auto& child : children)
      {
        if (child->item.id == id)
          return this;
        if (auto result = child->FindParent(id))
          return result;
      }
      return nullptr;
    };

    Node* Find(NodeId id)
    {
      if (item.id == id)
        return this;
      for (auto& child : children)
        if (auto result = child->Find(id))
          return result;
      return nullptr;
    };

    void Render(void (*renderItem)(Item*, void*), void* data)
    {
      ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      bool hasChildren = children.size() > 0;

      ImGuiTreeNodeFlags treeFlags = s_TreeNodeBaseFlags | item.flags;

      if (!hasChildren)
        treeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

      std::string label  = (item.label.size() == 0 ? ("##HierarchyItem:" + std::to_string(item.id)) : item.label);
      bool        opened = ImGui::TreeNodeEx(label.c_str(), treeFlags);

      if (item.onRenderItem)
        item.onRenderItem(&item);
      else
        renderItem(&item, data);

      if (hasChildren && opened)
      {
        for (auto& child : children)
          child->Render(renderItem, data);

        ImGui::TreePop();
      }

      ImGui::PopStyleVar();
    }
  };

private:
  std::unique_ptr<Node> m_Root           = std::make_unique<Node>();
  void*                 m_RenderItemData = nullptr;
  void (*m_RenderItem)(Item*, void*)     = nullptr;

  std::vector<NodeId> m_PendingRemovals{};
  std::vector<Item>   m_PendingAdditions{};

public:
  Hierarchy()  = default;
  ~Hierarchy() = default;

  void Add(const Item& item)
  {
    m_PendingAdditions.push_back(item);
  }

  void Remove(NodeId id)
  {
    m_PendingRemovals.push_back(id);
  };

  void Clear()
  {
    m_Root->children.clear();
  };

  void OnRenderItem(void (*callback)(Item*, void*))
  {
    m_RenderItem = callback;
  }

  void SetRenderItemData(void* renderItemData)
  {
    m_RenderItemData = renderItemData;
  }

  void Render(const std::string& id, void* renderItemData = nullptr)
  {
    if (renderItemData)
      m_RenderItemData = renderItemData;

    if (ImGui::BeginTable(id.c_str(), 1, s_TableFlags))
    {
      for (auto& child : m_Root->children)
        child->Render(m_RenderItem, m_RenderItemData);

      ImGui::EndTable();
    }

    if (!m_PendingRemovals.empty())
    {
      std::sort(m_PendingRemovals.begin(), m_PendingRemovals.end());
      m_PendingRemovals.erase(std::unique(m_PendingRemovals.begin(), m_PendingRemovals.end()), m_PendingRemovals.end());

      for (NodeId removeId : m_PendingRemovals)
        if (!m_Root->Remove(removeId))
          std::cerr << "Warning: tried to remove non-existent id " << removeId << "\n";

      m_PendingRemovals.clear();
    }

    if (!m_PendingAdditions.empty())
    {
      for (Item item : m_PendingAdditions)
        m_Root->Add(item);

      m_PendingAdditions.clear();
    }
  }
};