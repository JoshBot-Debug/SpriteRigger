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

class JointRenderSystem : public ECS::System {
private:
  struct Joint {
    glm::vec2 position = glm::vec2(0.0f);
    float boneThickness = 0.0f;
    glm::vec4 color = glm::vec4(0.0f);
  };

private:
  GLuint m_VAO = 0, m_VBO = 0;
  Shader *m_Shader = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;
  std::vector<Joint> m_Buffer;

public:
  ~JointRenderSystem() {
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
        .name = "joint",
        .vertex = (EXE_DIRECTORY + "/../src/Assets/Shaders/joint.vert").c_str(),
        .fragment =
            (EXE_DIRECTORY + "/../src/Assets/Shaders/joint.frag").c_str(),
    });

    // Create and bind the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    float quadVerts[8] = {
        -1.0f, -1.0f, // bottom-left
        1.0f,  -1.0f, // bottom-right
        -1.0f, 1.0f,  // top-left
        1.0f,  1.0f   // top-right
    };

    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    // layout(location = 0) in vec2 a_corner;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glVertexAttribDivisor(0, 0);

    // Now create the per-instance buffer (Joint instances)
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // layout(location = 1) in vec2 a_position;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Joint),
                          (void *)offsetof(Joint, position));
    glVertexAttribDivisor(1, 1);

    // layout(location = 2) in float a_boneThickness;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Joint),
                          (void *)offsetof(Joint, boneThickness));
    glVertexAttribDivisor(2, 1);

    // layout(location = 3) in vec4 a_color;
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Joint),
                          (void *)offsetof(Joint, color));
    glVertexAttribDivisor(3, 1);

    // Clean up
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Update() override {
    if (m_Registry->HasChanged<CBone>()) {
      auto bones = m_Registry->Get<CBone>();

      m_Buffer.clear();

      for (auto b : bones) {
        m_Buffer.emplace_back(Joint{
            .position = b->start,
            .boneThickness = b->thickness,
            .color = b->sColor,
        });
        m_Buffer.emplace_back(Joint{
            .position = b->end,
            .boneThickness = b->thickness,
            .color = b->eColor,
        });
      }

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, m_Buffer.size() * sizeof(Joint),
                   m_Buffer.data(), GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(m_VAO);
    m_Shader->bind("joint");

    m_Shader->setUniformMatrix4fv("u_ViewProjection",
                                  m_Camera->GetViewProjectionMatrix());

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)m_Buffer.size());

    m_Shader->unbind();
    glBindVertexArray(0);
  }
};
