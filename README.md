# Final-Project-behenry-aemara
CS 2300 - Final Project
Anne Mara & Brenna Henry


# Project Summary
This project visualizes real-world elevation data from the Vermont Open Geodata Portal by converting a Digital Elevation Model (DEM) in .asc format to make a 3D triangle mesh, displayed using OpenGL.

The program loads the DEM file, parses its elevation values, and constrcuts a downsampled mesh, which then uploads the mesh to the GPU via VAO methods, then renders the terrain with shaders. The final product being a 3D topographic model of Vermont.

This project demonstrates geospatial data visualization:
- Reading geographic data (Entirety of Vermont)
- Building mesh geometry
- Rendering data as 3D using OpenGL

# Data Source
Dataset: ElevationDEM_DEM24 (we changed to vt_data.asc)

Source: Vermont Open Geodata Portal (https://maps.vcgi.vermont.gov/gisdata/metadata/ElevationDEM_DEM24.htm)

Units: Elevation in feet which we converted to meters for clarity

* Note: Dataset was too large to include in github, instead, we stored the data locally.



# How to Run:
## Installations Required:
```
- GLFW
- GLAD
- GLM
- CMake
```
## How to Build: 
In the terminal:
```
mkdir build

cd build

cmake ..

cmake --build .

./FinalProject
```
# Analysis
### Why did you choose this data set?

We chose this data set because it provides real world, hgih-quality data for the entire state, which felt practical and useful.


### Challenges expected versus challenges faced?

Challenges we expected to face:
- Parsing .asc file correctly
- Cleaning the data
- Camera and matrix math for the view projection

Challenges we actually faced:
- File path and Cmake bugs
- Working across multiple branches/handling merge conflicts
- Coordinate scaling and visual clarity


### How could your 3D model be used?
This visual would be beneficial for mapping and GIS visualization, interactive displays involving the topology of Vermont, and even for tourism and evnironmental science related fields.





### Explanation of how the files interact
- Engine.cpp/Engine.h: controls the entire application by loading GLFW OpenGL, shaders, and handles the camera movements and input

- Terrain.cpp/Terrain.h: Loads the DEM (.asc file), generates the mesh grid, and creates VAO, VBO, and EBO to draw

- Shaders: Engine loads shaders and passes them to Terrain in order to color the Terrain



# Self Evaluation:
### Complexity / Usability - 80/80 points

Main program uses
many concepts
from all modules;
complex enough
for a large final
project (double the
size of an open
ended)



### Interactivity - 40/40 points

Program is fully
interactive in ways
that enhance the
functionality of the
program



### User Experience - 40/40 points

Program is
user-friendly,
intuitive, and runs
well



### ReadME - 20/20 points

Clear, concise,
well-formatted
README with all
requirements; includes
an analysis and
conclusions from the
model




# Rubric

### Complexity / Usability - 80 points
- 0 pts: No main function; program does not run  
- 20 pts: Incomplete, buggy  
- 40 pts: Limited complexity  
- 60 pts: Complex, minor bugs  
- 80 pts: Uses concepts from all modules, runs with little or no flaws 


### Interactivity - 40 points
- 0 pts: No input  
- 10 pts: Poor validation  
- 20 pts: Some flaws with input
- 30 pts: Mostly validated
- 40 pts: Fully interactive  



### User Experience - 40 points
- 0 pts: Not usable  
- 10 pts: Not intuitive  
- 20 pts: Usable with flaws  
- 30 pts: Smooth running with minor flaws
- 40 pts: Intuitive and polished with no flaws



### README - 20 points
- -20 pts: No README  
- -10 pts: Missing parts  
- 0 pts: Meets requirements   
- 20 pts: Clear, well-structured, complete, with full analysis


### Demo Video
- -20 pts: N/A
- -10 pts: Basic demo only  
- 0 pts: Multiple runs, validation, code shown 


### Lifespan
- -10 pts: All commits in 96 hours  
- -5 pts: Commits span < 9 days  
- 0 pts: Commits over 9+ days  

### Documentation
- -20 pts: None
- -10 pts: Some comments
- 0 pts: Clear & organized comments and documentation









