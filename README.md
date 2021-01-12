# Quartz Engine
Quartz Engine is a fully 3D game engine built in C++ using Vulkan.
This is primarily a personal project.

## Features (WIP)
- [ ] Core
  - [x] Sub-Systems
  - [x] Multi-module
  - [ ] Custom memory allocation
  - [ ] Cross-platform
    - [x] Windows
    - [ ] Linux
    - [ ] MacOS
- [ ] Windowing System
  - [x] Single window
  - [ ] Dynamicly resizeable swapchain
  - [ ] Multiple windows
- [x] Input
  - [x] Input callbacks
  - [x] Controller suport
  - [x] Device connection callbacks
  - [x] Windows DirectInput support
- [ ] Graphics System
  - [x] Vulkan support
  - [ ] OpenGL support
  - [x] Geometry
  - [x] Textures/Normals
  - [x] Vert/Frag Shaders
  - [ ] Geom/Tess Shaders
  - [ ] Mesh Shaders
  - [ ] Ray Tracing Shaders
  - [ ] Compute shaders
  - [ ] Physically Based Rendering
  - [ ] Shader reflection
  - [ ] Pipeline state serialization
- [x] Entity Component System
  - [x] Multiple 'worlds'
  - [x] Component-system registry
  - [x] Itterable by component filters
  - [x] Contiguous memory for component types
  - [x] Template meta-programming
- [ ] Custom Standard Libraries
  - [x] String
    - [x] Sub-String
    - [x] Unicode support
  - [x] Array (vector)
  - [x] Linked List
  - [x] Map (Hashmap, implemented robin-hood)
  - [x] Set (Hashset)
  - [x] Stack
  - [x] Tuple
  - [ ] Heap
  - [x] BitSet
  - [x] Pool
  - [x] Buffer (byte, int, etc)
  - [x] Reference pointer
- [ ] Math
  - [x] Vector2/3/4
  - [x] Matrix4
  - [ ] Matrix 2/3/etc
  - [x] Quaternions
  - [x] Point/Bounds
  - [ ] SIMD
- [ ] Reflection
  - [x] Compile-time type ids
  - [ ] Full run-time object reflection
- [ ] Editor
- [ ] Additional
  - [x] OBJ support
  - [ ] GLTF support
  - [ ] Custom model format
  - [ ] Serialization
## Dependencies
* C++17 or higher
* Vulkan SDK
  * Any Vulkan 1.2+ SDK with VULKAN_SDK path set
* Windows SDK
  * Any Windows 10 SDK (you may need to retarget solution)
