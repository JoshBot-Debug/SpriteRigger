#pragma once

#include <array>
#include <cstring>
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
  GLuint m_VAO = 0, m_VBO = 0, m_QuadVBO = 0;
  Shader *m_Shader = nullptr;
  ECS::Registry *m_Registry = nullptr;
  OrthographicCamera *m_Camera = nullptr;

  void *m_Buffer = nullptr;
  GLsizei m_Instances = 0;

public:
  void Free() {
    if (m_VBO)
      glDeleteBuffers(1, &m_VBO);
    if (m_QuadVBO)
      glDeleteBuffers(1, &m_QuadVBO);
    if (m_VAO)
      glDeleteVertexArrays(1, &m_VAO);

    m_VAO = 0;
    m_VBO = 0;
    m_Buffer = nullptr;
    m_Shader = nullptr;
    m_Registry = nullptr;
    m_Camera = nullptr;
    m_Instances = 0;
  }

  void Initialize(ECS::Registry *registry, Shader *shader,
                  OrthographicCamera *camera) {
    m_Camera = camera;
    m_Shader = shader;
    m_Registry = registry;

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

    glGenBuffers(1, &m_QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    // layout(location = 0) in vec2 a_corner;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glVertexAttribDivisor(0, 0);

    // Now create the mapped buffer
    {
      glGenBuffers(1, &m_VBO);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

      GLsizeiptr bufferSize = 1024 * sizeof(Bone);

      glBufferStorage(GL_ARRAY_BUFFER, bufferSize, nullptr,
                      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT |
                          GL_MAP_COHERENT_BIT);

      m_Buffer = glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize,
                                  GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT |
                                      GL_MAP_COHERENT_BIT);
    }

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

    if (m_Registry->AnyChanged<CBone>()) {
      m_Registry->ClearChanged<CBone>();

      std::vector<Bone> buffer;

      for (auto [_, cBone] : m_Registry->Get<CBone>()) {
        Bone &bone = buffer.emplace_back();

        bone.color = cBone->color;
        bone.start = cBone->joints[CBone::StartJoint].position;
        bone.end = cBone->joints[CBone::EndJoint].position;
        bone.sColor = cBone->joints[CBone::StartJoint].color;
        bone.eColor = cBone->joints[CBone::EndJoint].color;
        bone.thickness = cBone->thickness;
      }

      m_Instances = static_cast<GLsizei>(buffer.size());
      std::memcpy(m_Buffer, buffer.data(), buffer.size() * sizeof(Bone));
    }

    if (!m_Instances)
      return;

    glBindVertexArray(m_VAO);
    m_Shader->bind("bone");

    m_Shader->setUniformMatrix4fv("u_ViewProjection",
                                  m_Camera->GetViewProjectionMatrix());

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_Instances);

    m_Shader->unbind();
    glBindVertexArray(0);
  }
};
