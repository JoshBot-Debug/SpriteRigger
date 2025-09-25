#include "ViewportLayer.h"

#include "Application/Components.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

const std::string EXE_DIRECTORY = GetExecutableDirectory();

ViewportLayer::ViewportLayer(State *state) : m_State(state) {}

void ViewportLayer::OnAttach() {
  m_Shader.create({
      .name = "default",
      .vertex = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.vert").c_str(),
      .fragment = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.frag").c_str(),
      .geometry = (EXE_DIRECTORY + "/../src/Assets/Shaders/bone.geom").c_str(),
  });

  // Create and bind the VAO
  glGenVertexArrays(1, &boneVAO);
  glBindVertexArray(boneVAO);

  // one small VBO used only to feed a single vertex (we call
  // glDrawArraysInstanced)
  GLuint dummyVBO;
  glGenBuffers(1, &dummyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, dummyVBO);
  float dummy = 0.0f;
  glBufferData(GL_ARRAY_BUFFER, sizeof(dummy), &dummy, GL_STATIC_DRAW);

  // attribute 0: a placeholder vertex attribute (vec2/vacant)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glVertexAttribDivisor(0, 0); // per-vertex

  // Now create the per-instance buffer
  glGenBuffers(1, &boneInstanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, boneInstanceVBO);
  // allocate no data yet; we'll bufferSubData later
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  // layout(location = 0) in vec2 a_start;
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, start));
  glVertexAttribDivisor(1, 1);

  // layout(location = 1) in vec2 a_end;
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, end));
  glVertexAttribDivisor(2, 1);

  // location = 2 thickness
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, thickness));
  glVertexAttribDivisor(3, 1);

  // location = 3 color (vec4)
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(CBone),
                        (void *)offsetof(CBone, color));
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);
}

void ViewportLayer::OnUpdate(float deltaTime) {

  auto registry = ServiceLocator::Get<Registry>();

  if (!registry->HasChanged<CBone>())
    return;

  registry->ClearChanged<CBone>();

  std::vector<CBone *> bones = registry->Get<CBone>();

  m_Bones.resize(bones.size());

  for (auto &bone : bones)
    m_Bones.emplace_back(*bone);
}

void ViewportLayer::OnRender() {
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);

  ImVec2 viewportSize = ImGui::GetContentRegionAvail();

  // Create the framebuffer
  if (fbo == 0) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // create color attachment
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewportSize.x, viewportSize.y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorTex, 0);

    // create depth buffer
    glGenRenderbuffers(1, &depthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewportSize.x,
                          viewportSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, depthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cerr << "Framebuffer not complete!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, viewportSize.x, viewportSize.y);
  glClearColor(0.1f, 0.1f, 1.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Upload instance data
  glBindBuffer(GL_ARRAY_BUFFER, boneInstanceVBO);
  glBufferData(GL_ARRAY_BUFFER, m_Bones.size() * sizeof(CBone), m_Bones.data(),
               GL_DYNAMIC_DRAW);

  // draw
  glBindVertexArray(boneVAO);

  // set uniforms
  m_Shader.bind("default");
  m_Shader.setUniform2f("u_screenSize",
                        glm::vec2(viewportSize.x, viewportSize.y));

  // draw single point instanced per bone
  glDrawArraysInstanced(GL_POINTS, 0, 1, (GLsizei)m_Bones.size());

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
  m_Shader.unbind();

  ImGui::Image((void *)(intptr_t)colorTex, viewportSize);

  ImGui::End();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {}

void ViewportLayer::Restore(Serializer &serializer) {}
