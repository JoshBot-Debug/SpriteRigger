#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "imgui.h"

#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Application/Components.h"
#include "Shader/Shader.h"

#include "glad/glad.h"

#include "Utility.h"

#include "Camera/OrthographicCamera.h"
#include "Common.h"

class BoneRenderSystem : public ECS::System {
private:
  struct Bone {
    glm::vec4 color = glm::vec4(0.0f);
    glm::vec2 start = glm::vec2(0.0f);
    glm::vec2 end = glm::vec2(0.0f);
    glm::vec4 sColor = glm::vec4(0.0f);
    glm::vec4 eColor = glm::vec4(0.0f);
    float thickness = 0.0f;
  };

private:
  GLuint m_VAO = 0, m_VBO = 0;
  Shader *m_Shader = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;
  std::vector<Bone> m_Buffer;

private:
  void UpdateColor(ECS::Registry *registry, CBone *bone, CHovered *hovered,
                   float deltaTime) {
    auto &c = bone->color;
    auto &s = bone->joints[CBone::StartJoint].color;
    auto &e = bone->joints[CBone::EndJoint].color;

    auto ch = Colors::DEFAULT;
    auto sh = Colors::DEFAULT;
    auto eh = Colors::DEFAULT;

    if (hovered && hovered->target == CBone::StartJoint)
      sh = Colors::HIGHLIGHT;
    else if (hovered && hovered->target == CBone::EndJoint)
      eh = Colors::HIGHLIGHT;
    else if (hovered && hovered->target == CBone::Shaft) {
      ch = Colors::HIGHLIGHT;
      sh = Colors::HIGHLIGHT;
      eh = Colors::HIGHLIGHT;
    }

    if (ECS::Mutate<CBone, glm::vec4>(
            registry, c, glm::mix(c, ch, ANIMATION_SPEED * deltaTime)) &&
        ECS::Mutate<CBone, glm::vec4>(
            registry, s, glm::mix(s, sh, ANIMATION_SPEED * deltaTime)) &&
        ECS::Mutate<CBone, glm::vec4>(
            registry, e, glm::mix(e, eh, ANIMATION_SPEED * deltaTime))) {
      if (!hovered)
        m_Registry->ClearChanged<CHovered>();
    }
  }

public:
  ~BoneRenderSystem() {
    m_VAO = 0;
    m_VBO = 0;
    m_Shader = nullptr;
    m_Registry = nullptr;
    m_Camera = nullptr;
  }

  void Initialize(ECS::Registry *registry, Shader *shader,
                  OrthographicCamera *camera) {
    m_Registry = registry;
    m_Shader = shader;
    m_Camera = camera;

    const std::string EXE_DIRECTORY = GetExecutableDirectory();

    m_Shader->create({
        .name = "bone",
        .vertex = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.vert").c_str(),
        .fragment =
            (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.frag").c_str(),
    });

    // Create and bind the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float quadVerts[8] = {0.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f};

    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    // layout(location = 0) in vec2 a_corner;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glVertexAttribDivisor(0, 0);

    // Now create the per-instance buffer (CBone instances)
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // layout(location = 1) in vec2 a_start;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, start));
    glVertexAttribDivisor(1, 1);

    // layout(location = 2) in vec2 a_end;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, end));
    glVertexAttribDivisor(2, 1);

    // layout(location = 3) in float a_thickness;
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, thickness));
    glVertexAttribDivisor(3, 1);

    // layout(location = 4) in vec4 a_color;
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, color));
    glVertexAttribDivisor(4, 1);

    // layout(location = 5) in vec4 a_sColor;
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, sColor));
    glVertexAttribDivisor(5, 1);

    // layout(location = 6) in vec4 a_sColor;
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Bone),
                          (void *)offsetof(Bone, eColor));
    glVertexAttribDivisor(6, 1);

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    if (m_Registry->AnyChanged<CBone, CHovered>()) {
      m_Registry->ClearChanged<CBone>();

      m_Buffer.clear();

      auto [cBoneC, cHoveredC] = m_Registry->HasChanged<CBone, CHovered>();

      for (auto entity : m_Registry->GetEntities("bone")) {
        Bone &buffer = m_Buffer.emplace_back();

        auto bone = entity->Get<CBone>();
        auto hovered = entity->Get<CHovered>();

        UpdateColor(m_Registry, bone, hovered, data->deltaTime);

        buffer.color = bone->color;
        buffer.start = bone->joints[CBone::StartJoint].position;
        buffer.end = bone->joints[CBone::EndJoint].position;
        buffer.sColor = bone->joints[CBone::StartJoint].color;
        buffer.eColor = bone->joints[CBone::EndJoint].color;
        buffer.thickness = bone->thickness;
      }

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, m_Buffer.size() * sizeof(CBone),
                   m_Buffer.data(), GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(m_VAO);
    m_Shader->bind("bone");

    m_Shader->setUniformMatrix4fv("u_ViewProjection",
                                  m_Camera->GetViewProjectionMatrix());

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)m_Buffer.size());

    m_Shader->unbind();
    glBindVertexArray(0);
  }
};
