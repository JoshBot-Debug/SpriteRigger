Renderer renderer;

/**
 * This should setup positions, normals and texCoords;
 * 
 * To accomplish this, we'll use create a struct Vertex to hold that data.
 * The key here is to use offsetof to sepcify the byte location of the data.
 * 
 * If there isn't any data for normals, give it a default value. In the shader, if the value is the default, skip it or whatever.
 */

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
glEnableVertexAttribArray(1);

glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
glEnableVertexAttribArray(2);

renderer.loadFBX("/path/to/file");

renderer.