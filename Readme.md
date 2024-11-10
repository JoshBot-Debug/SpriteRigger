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