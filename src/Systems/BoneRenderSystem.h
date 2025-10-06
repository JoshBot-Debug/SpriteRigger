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

const std::string EXE_DIRECTORY = GetExecutableDirectory();

class BoneRenderSystem : public ECS::System {
private:
  GLuint m_VAO = 0, m_VBO = 0;
  Shader *m_Shader = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;
  std::vector<CBone> m_Buffer;

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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                          (void *)offsetof(CBone, start));
    glVertexAttribDivisor(1, 1);

    // layout(location = 2) in vec2 a_end;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                          (void *)offsetof(CBone, end));
    glVertexAttribDivisor(2, 1);

    // layout(location = 3) in float a_thickness;
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CBone),
                          (void *)offsetof(CBone, thickness));
    glVertexAttribDivisor(3, 1);

    // layout(location = 4) in vec4 a_color;
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(CBone),
                          (void *)offsetof(CBone, color));
    glVertexAttribDivisor(4, 1);

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Update() override {
    if (m_Registry->HasChanged<CBone>()) {
      m_Registry->ClearChanged<CBone>();

      auto bones = m_Registry->Get<CBone>();
      m_Buffer.clear();
      m_Buffer.reserve(bones.size());
      for (auto b : bones)
        m_Buffer.emplace_back(*b);

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
