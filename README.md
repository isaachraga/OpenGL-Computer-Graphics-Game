# OpenGL Computer Graphics Game
Robot-shooting game made for a computer graphics class
 # OpenGL Rendering Engine  
_A modular C++ OpenGL renderer demonstrating real-time lighting, texturing, and a clean graphics pipeline implementation._

## Overview
This project is a custom OpenGL rendering engine written in modern C++ (GLFW + GLEW/GLAD).  
It was built to demonstrate a clear understanding of:

- The graphics rendering pipeline  
- Lighting models  
- Texture mapping and sampling  
- Coordinate transformations and matrices  
- Rendering architecture and engine structure  

The project includes custom shader code, a modular renderer, and utilities for textures, meshes, windowing, and text rendering.

---

## Demo (Add Your Media Here)
- **Demo Video:** _Add a link_  
- **GIFs / Screenshots:**  
  - ![Rendering Example](link_here)  
  - ![Lighting](link_here)

---

## Key Features
- **Full Graphics Pipeline Implementation**  
  Vertex processing → primitive assembly → clipping → rasterization → fragment shading.

- **Per-Pixel Lighting**  
  Phong or Blinn–Phong shading implemented in fragment shaders.

- **Texture Mapping System**  
  Support for diffuse textures, sampling modes, and texture filtering.

- **Normal Handling for Lighting**  
  Normal calculation per vertex + normal interpolation across fragments.

- **Custom Shader Management**  
  Hot-reloadable GLSL vertex and fragment shaders.

- **Matrix Transformation Pipeline**  
  World → View → Projection transformations with configurable camera system.

- **Modular Engine Architecture**  
  Organized into subsystems for input, rendering, object management, and utilities.

- **Text Rendering Utility**  
  Ability to draw 2D/3D text on screen using FreeType or bitmap fonts.

---

## Skills Demonstrated
- C++ design (RAII, encapsulation, header/module breakdown)
- GPU pipeline understanding  
- GLSL shader development  
- Texture sampling, filtering, aliasing concepts  
- Normal/tangent-space lighting  
- Coordinate transformations, view volumes, clipping  
- Debugging rendering bugs (coordinate systems, depth issues, texture alignment)
- Practical experience with OpenGL API

---

## Architecture

## How to Run

### **Prerequisites**
- C++17 or later  
- CMake  
- GLFW  
- GLAD or GLEW  
- (Optional) FreeType for text rendering  

### **Setup**


## Controls 

-WASD – Move camera

-Mouse – Look around

-Esc – Quit

## Important Code Locations

Graphics/Renderer.cpp — top-level pipeline implementation

Graphics/Shader.cpp — GLSL compilation, error logging

Graphics/Texture.cpp — wrapping, filtering, mipmap generation

Graphics/Mesh.cpp — VAO/VBO setup and attribute layout

Graphics/Lighting.glsl — per-pixel lighting calculations

UI/TextRenderer.cpp — 2D text using textures and quads

main.cpp — scene setup + render loop



## Technologies Used

C++17

OpenGL 3.3+

GLFW

GLAD / GLEW


## License

MIT License

## Contact
-Isaac Hraga
-GitHub: https://github.com/isaachraga
-LinkedIn: www.linkedin.com/in/isaac-hraga-5b7535b2
