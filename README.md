# aie-opengl-renderer

An OpenGL renderer written for the AIE Graphics Unit, using GLFW.

This renderer has support for:
- PBR shading/lighting
  - Diffuse irradiance mapping
  - Specular IBL
  - Works with any HDRi skybox (that has been separated into cubemaps
- Skybox rendering
- Bloom
- Post Processing Stack
- Fog
- Model and Texture loading during runtime
- Minimal material system that works with any shader, allows for texture and uniform properties

The renderer has a rudimentary Camera system, including a camera stack.
Dear ImGui is used as a debugging tool to place lights, load and unload models/textures, edit materials, override materials of objects, create instances of meshes, etc.
The repository was mostly used as a glorified portable hard drive, so it is very messy and working files are still in the folder.
