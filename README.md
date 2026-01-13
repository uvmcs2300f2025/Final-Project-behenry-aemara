# Vermont DEM 3D Terrain Visualization  
CS 2300 — Final Project  
Authors: Anne Mara & Brenna Henry

---

## Overview  
This project visualizes real-world elevation data from the Vermont Open Geodata Portal by converting a Digital Elevation Model (DEM) in `.asc` format into a 3D mesh rendered using OpenGL.

The system:
- Parses DEM elevation values
- Constructs a downsampled triangle mesh
- Uploads the mesh to the GPU via VAO/VBO/EBO buffers
- Renders the terrain using custom GLSL shaders

The final output is an interactive 3D topographic model of Vermont built from real geospatial elevation data.

---

## Core Capabilities  
- Ingests and parses statewide DEM data  
- Converts raster elevation values into mesh geometry  
- Downsamples large geospatial datasets for real-time rendering  
- Uses GPU rendering via OpenGL pipeline  
- Interactive camera navigation

---

## Technologies Used  
- C++  
- OpenGL (VAO, VBO, EBO)  
- GLSL Shaders  
- GLFW  
- GLAD  
- GLM  
- CMake  
- Vermont Open Geodata DEM24 Dataset  

---

## Data Source  
**Dataset:** ElevationDEM_DEM24 (converted locally to `vt_data.asc`)  
**Source:** Vermont Open Geodata Portal  
**Units:** Elevation originally provided in feet (converted to meters)  

Note: DEM dataset excluded from repository due to file size; stored locally.

---

## Running the Project  

### Dependencies  
- GLFW
- GLAD
- GLM
- CMake

### Build Instructions  
- mkdir build
- cd build
-cmake ..
- cmake --build .
- ./FinalProject

---

## Technical Architecture  
**Engine.cpp / Engine.h**  
Initializes OpenGL and GLFW, loads shaders, and manages camera & user input.

**Terrain.cpp / Terrain.h**  
Parses DEM file, constructs triangle mesh geometry, generates VAO/VBO/EBO, and renders terrain.

**Shaders**  
Custom GLSL shader programs for vertex transformation and fragment coloring.

---

## Motivation  
We selected statewide DEM data because it provides high-resolution, real-world elevation coverage and supports meaningful visualization and GIS-related applications.

---

## Challenges  

**Expected challenges:**  
- Parsing `.asc` DEM format  
- Matrix math for camera and projection  
- Handling large geospatial datasets  

**Actual challenges:**  
- CMake configuration & file path issues  
- Merge conflicts during collaboration  
- Coordinate scaling for visual clarity  

---

## Potential Applications  
This type of 3D model is useful for:  
- Geospatial analysis & mapping  
- Environmental & conservation planning  
- Tourism and interactive education  
- Scientific visualization and GIS tooling  

---

## Result  
The project successfully renders a full 3D terrain model of Vermont using real elevation data, demonstrating an end-to-end pipeline from geospatial raster to GPU-rendered visualization.
