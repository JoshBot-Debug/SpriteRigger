# Application Development Plan: Pivot Animator-like Tool

# Entity-Component-System Design
- [ ] Entity is a uint32
- [ ] A registry is used to create an Entity. Entity entity = registry.create();
- [ ] Add components to an entity via registry registry.emplace<StructComponent>(entity, ...constructor args);
- [ ] All comonents must have a default constructor and a copy constructor
- [ ] To get entities in render, you need to call registry.view<StructComponent>(), it will return all entities that have a StructComponent
- [ ] Need to create a registry.get<StructComponent>(entity);
- [ ] https://www.youtube.com/watch?v=D4hz0wEB978


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