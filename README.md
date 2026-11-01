# CENG 477 — Ray tracer

Homework 1 for **CENG 477 Introduction to Computer Graphics** at METU: a basic ray tracer in **C++**, done as a
pair project.

## What it does

- Reads a scene from an XML file with [TinyXML-2](https://github.com/leethomason/tinyxml2): cameras, point
  lights, materials and the geometry.
- Supports **spheres**, **triangles** and **triangle meshes**.
- Shades with **ambient**, **diffuse** and **specular** (Blinn–Phong) lighting, casts **shadow** rays, and
  follows **mirror reflections** up to the scene's recursion depth.
- Writes one image per camera as a **PPM** file.

The assignment brief is `ceng477hw1.pdf`.

## Layout

| File | What it is |
|---|---|
| `main.cpp` | entry point: loads the scene file given as the argument and renders it |
| `Scene` | parses the XML and runs the ray tracing for every camera |
| `Camera`, `Ray` | the view setup and primary rays |
| `Shape` | `Sphere`, `Triangle` and `Mesh`, with their ray intersections |
| `Material`, `Light` | material coefficients and point lights |
| `Image` | the pixel buffer and the PPM writer |
| `defs.h` | the vector and intersection-result types |
| `tinyxml2.*` | the XML parser (third party) |

## Running it

```bash
make
./raytracer scene.xml
```

The `raytracer` binary in the repo is a prebuilt Linux build.
