Vermont DEM 3D Terrain Visualization
CS 2300 — Final Project
Anne Mara & Brenna Henry
Overview
This project visualizes real-world elevation data from the Vermont Open Geodata Portal by converting a Digital Elevation Model (DEM) in .asc format into a 3D mesh rendered using OpenGL. The system parses DEM elevation values, constructs a downsampled triangle mesh, uploads the mesh to the GPU via VAO/VBO/EBO buffers, and renders the result with custom shaders to produce an interactive topographic model of Vermont.
This project integrates geospatial data processing, graphics programming, and GPU-based rendering.
Core Capabilities
Ingests and parses Vermont statewide DEM data
Generates 3D mesh geometry from raster elevation data
Downsamples large geospatial datasets for real-time rendering
Uploads geometry to GPU via OpenGL pipeline
Renders terrain using shader programs and an interactive camera
Technologies Used
C++
OpenGL (VAO, VBO, EBO)
GLSL Shaders
GLFW
GLAD
GLM
CMake
Vermont Open Geodata DEM24 dataset
Data Source
Dataset: ElevationDEM_DEM24 (converted to vt_data.asc)
Source: Vermont Open Geodata Portal
Units: Elevation in feet (converted to meters for clarity)
Note: DEM dataset stored locally due to size constraints.
Running the Project
Dependencies
GLFW
GLAD
GLM
CMake
Build Instructions
mkdir build
cd build
cmake ..
cmake --build .
./FinalProject
Technical Architecture
Engine.cpp / Engine.h
Initializes OpenGL and GLFW, loads shaders, manages camera and user input.
Terrain.cpp / Terrain.h
Parses DEM file, constructs 3D triangle mesh, generates VAO/VBO/EBO, and renders terrain geometry.
Shaders
Custom GLSL shader programs for vertex transformations and fragment coloring.
Motivation
We selected statewide DEM data because it provides high-quality, real-world geospatial coverage and supports meaningful visualization and analysis applications.
Challenges
Expected challenges:
Parsing .asc DEM format
Camera, projection matrices, and rendering math
Resource constraints with large datasets
Encountered challenges:
CMake and file path issues across machines
Branch merge conflicts during collaboration
Coordinate scaling affecting visual clarity
Potential Applications
3D terrain visualization has value across:
Geospatial analysis and mapping
Environmental and conservation planning
Tourism & education (interactive displays)
Scientific and GIS data visualization
Result
The final product renders a full 3D model of Vermont’s terrain using real elevation data, demonstrating end-to-end processing from geospatial raster to GPU-rendered visualization.
