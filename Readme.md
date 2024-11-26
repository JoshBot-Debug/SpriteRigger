# TODO
- [x] Move all managers to App.h, ShaderManager, Model (create/load a model)
- [ ] Bind once, use a cache, if a shader/buffer is bound, skip the opengl call
- [x] Input needs to have a mouse offset .setMouseOffset which I can call when the target viewport is resized. This removes the viewport dependency from other scenes
- [ ] Pass camera from App to viewport
- [ ] Use one buffer for all vertices and indices
- [ ] When drawing, you can specify the offset in bytes to the indices

# Application Development Plan: Pivot Animator-like Tool

- [x] State Serializer needs to be a library in Application.
- [x] State Serializer will save data in a map that takes in a key and the value is string.
- [x] This map will be written to disk and read.
- [x] Recent files will basically be the same thing.
- [x] i.e. We will have two StateSerializers once for app state, and one for recent files,
- [x]      this is because recent files need to be saved at the application root, where as project state will be saved in the project directory.
- [x] Expected usage
  - StateSerializer *state = new StateSerializer();
  - state->setSaveFileName("Akari");
  - state->setSaveFileExtension("sprig" / "recent");
  - state->setSaveFileDirectory("/path/to/save/file/dir");
  - state->map("key", "value");
  - state->write(); // Will write the save file into state
  - OR
  - state->write(SaveFile{"Akari", "sprig", "/path/to/save/file/dir"}); // custom directory, does not update member in StateSerializer
  - 
  - state->read(); // Will read the save file into state
  - OR
  - state->read(SaveFile{"Akari", "sprig", "/path/to/save/file/dir"}); // custom directory, does not update member in StateSerializer
  - 

# Entity-Component-System Design
- [x] Entity is a uint32
- [x] A registry is used to create an Entity. Entity entity = registry.createEntity();
- [x] Add components to an entity via Entity entity.add<StructComponent>(entity, ...constructor args);
- [x] To get entities in render, you need to call (entity/registry).get<StructComponent>(), it will return all components that have a StructComponent


## Basic Version (1-2 Months)

### Week 1-2: Setup and Basic Features
- [x] - **Environment Setup**: Install SDL3 and set up your development environment.
- [x] - **Basic Window Creation**: Create a simple window using SDL3.
- [ ] - **Drawing Functionality**: Implement basic rigging functions to create a skeleton with joints and bones.

### Week 3: User Input Handling
- [ ] - **Mouse Input**: Allow users to click and drag to create and manipulate the skeleton.
- [ ] - **Keyboard Input**: Implement basic controls for selecting and deleting bones.

### Week 4: Animation Frame System
- [ ] - **Frame Management**: Create a system to manage frames (add, remove, and switch between frames).
- [ ] - **Basic Animation Playback**: Implement play, pause, and step-through functionality for animations.

### Week 5-6: Basic UI and Final Touches
- [ ] - **Simple UI**: Design a minimal user interface for tools and controls.
- [ ] - **Testing and Bug Fixing**: Test functionality and fix any major bugs.
- [ ] - **Documentation**: Write basic usage instructions.

---

## Intermediate Version (2-6 Months)

### Month 1: Enhancements to Basic Features
- **Advanced Drawing Tools**: Implement features for resizing, rotating, and copying figures.
- **Layers**: Add a layer system for better organization of animations.

### Month 2: Improved Animation System
- **Keyframe Animation**: Implement keyframes for more complex movements.
- **Easing Functions**: Introduce easing for smoother transitions.

### Month 3: UI/UX Improvements
- **Enhanced UI**: Improve the user interface with toolbars and icons.
- **Preview Feature**: Add a preview window to see animations without rendering.

### Month 4: Save/Load Functionality
- **File Management**: Implement save and load functions for user projects.
- **Export Options**: Allow basic export of animations (e.g., as images or simple video formats).

### Month 5-6: Testing and Polishing
- **User Testing**: Gather feedback from users to identify pain points.
- **Polishing**: Improve performance and usability based on feedback.
- **Documentation**: Create a more comprehensive user guide.

---

## Advanced Version (6+ Months)

### Month 1-2: Advanced Editing Features
- **Customizable Characters**: Allow users to create and save their custom stick figures.
- **Collision Detection**: Implement basic collision detection for interactive animations.

### Month 3: Community Features
- **User Community**: Consider features for sharing animations or user-generated content.
- **Tutorials**: Develop tutorials for new users to understand advanced features.

### Month 4: Robust Export Options
- **Export as GIF/Video**: Implement functionality to export animations as GIFs or videos.
- **Advanced File Formats**: Support for more complex file formats for saving projects.

### Month 5: Performance Optimization
- **Optimization**: Profile the application and optimize rendering and memory usage.
- **Cross-Platform Testing**: Ensure the application works seamlessly on all supported platforms.

### Month 6: Final Testing and Release
- **Beta Testing**: Conduct a beta testing phase with a larger user group.
- **Polishing**: Fix any final bugs and polish the user experience.
- **Launch**: Prepare for a public release, including marketing and distribution.


// Define position and color for each vertex
glm::vec2 position(0.0f, 0.0f);  // Bottom-left corner at (0, 0)
glm::vec3 color(1.0f, 0.0f, 0.0f); // Red color

float width = 1.0f;  // Width of the rectangle
float height = 1.0f; // Height of the rectangle

// Define the rectangle vertices (positions and colors)
unsigned int vertices[] = {
    // Positions               // Colors
    position.x,               position.y,              color.r, color.g, color.b, // Bottom-left corner
    position.x + width,       position.y,              color.r, color.g, color.b, // Bottom-right corner
    position.x + width,       position.y + height,     color.r, color.g, color.b, // Top-right corner
    position.x,               position.y + height,     color.r, color.g, color.b  // Top-left corner
};

unsigned int indices[] = {
    0, 1, 2,  // First triangle (bottom-left, bottom-right, top-right)
    0, 2, 3   // Second triangle (bottom-left, top-right, top-left)
};



unsigned int VAO, VBO, EBO;

// Generate and bind the VAO
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);
// The VAO is now bound and it is recording

// Generate and bind the VBO
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// Generate and bind the EBO (Element Buffer Object)
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// Define the position attribute (location 0) in our shader
(shader location, count, type, isNormalized, size, position)
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
glEnableVertexAttribArray(0);

// Define the color attribute (location 1)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);

// Unbind the VAO
// This will automaticall unbind the VBO & EBO, since it was recorded
// Using a VAO means you only need to bind the VAO before drawing, it will bind all the VBO's and EBO's that was recorded
// That's why they say it holds state.
glBindVertexArray(0);


// RENDER TIME
// Bind the VAO (this automatically binds the VBO and EBO)
glBindVertexArray(VAO);

// Draw the rectangle using the EBO by calling glDrawElements (Hence it draws the EBO that's currently bound)
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// Unbind the VAO
glBindVertexArray(0);


* The size of the bone, this is basically the collision box.
* The mesh may not intersect with the mouse but if the mouse is in
* the collision box, we know we are making contact.
* Checking the verticies to calculate if we are colliding is not possible since
* the data is stored in a buffer on the GPU. We do not want to store that data on the cpu.

// Updating transform
// Update when the verticies are the same, recreate only if they changed. (if a vertice was removed or added, etc)
glBufferSubData is used to update the vertex array buffer on the GPU.
Instancing and Uniforms are more efficient for many updates.
Instancing for updating many instance transforms,
Uniforms for updating every transform, ideal for camera movement, etc.


// Updating using instancing double buffer, single buffer is using only one buffer

// Create two instance buffers (double buffering)
GLuint instanceVBO1, instanceVBO2;
glGenBuffers(1, &instanceVBO1);
glGenBuffers(1, &instanceVBO2);

// Create a flag to toggle between the buffers each frame
GLuint currentInstanceVBO = instanceVBO1;  // Start with instanceVBO1

// Every frame, swap between the buffers
if (currentInstanceVBO == instanceVBO1)
    currentInstanceVBO = instanceVBO2;
else
    currentInstanceVBO = instanceVBO1;

// Map the current instance buffer to CPU memory
glBindBuffer(GL_ARRAY_BUFFER, currentInstanceVBO);
InstanceData* mappedData = (InstanceData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

// Update the data directly on the GPU
mappedData[0].position = entity1.transform.position;
mappedData[1].position = entity2.transform.position;
mappedData[0].color = entity1.color;
mappedData[1].color = entity2.color;

// Unmap the buffer to apply changes
glUnmapBuffer(GL_ARRAY_BUFFER);