#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader/shader.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>

// Simple terrain built from a heightmap (ASC) into a triangle mesh
class Terrain
{
public:
    // width/height are the number of grid cells; cellSize is world units per cell
    Terrain(Shader &shader, int width, int height, float cellSize);

    // Load ASCII grid (.asc) heightmap and rebuild mesh
    bool loadHeightmapASC(const std::string &filename);

    // Draw terrain with given camera matrices
    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

private:
    // We store a reference to the shader used to draw this terrain
    Shader &shader;
    glm::mat4 model{1.0f};

    int width;
    int height;
    float cellSize;
    float heightScale = 1.0f; // vertical exaggeration

    // [row][col] height samples from the ASC file
    std::vector<std::vector<float>> heightData;

    // Mesh data: 3 floats per vertex (x, y, z)
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // OpenGL buffer objects
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void generateMesh(); // build vertices/indices from heightData
    void initBuffers();  // upload data to OpenGL buffers
};

#endif // TERRAIN_H
