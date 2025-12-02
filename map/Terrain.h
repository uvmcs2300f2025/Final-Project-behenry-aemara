#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader/shader.h"
// #include "shapes/cube.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector, glm::vec3, glm::mat4;

// OUTLINE / TODOS

// 1. Store a reference to the shader
// 2. Store terrain dimensions and position
// 3. Store a vector of Cube objects
// 4. Build a grid of cubes - flat for now until it works, then add 3D modeling later
// 5. Draw all cubes using the shader

// Store a reference to the shader
struct color
{
    // RGBY colors but can change later
    float red;
    float green;
    float blue;
    float yellow;
};

// Store terrain dimensions and position
class Terrain
{
public:
    Terrain(Shader &shader, int width, int height, float cellSize);
    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

    // loading in asc file
    bool loadHeightmapASC(const std::string &filename);

private:
    Shader shader;
    // check the notation of this line? unsure if it needs an additional
    glm::mat4 model;

    int width;
    int height;
    float cellSize;
    // exspect to change this for best effects
    float heightScale = 1.0f;

    std::vector<std::vector<float>> heightData;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // unsure if this line is needed -> link source from notes HERE
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    // this is the way that i was gonna import our grid
    void generateMesh();
    void initVAO();
    void initVBO();
    void initEBO();

    // glm::mat4 model;
    // vector<Cube> tiles;
    // void initTiles();
};

#endif // TERRAIN_H