#ifndef TERRAIN_H
#define TERRAIN_H

#include "../shader/shader.h"

#include <vector>
#include <string>

#include <glm/glm.hpp>
class Terrain
{
public:
    Terrain(Shader &shader, int width, int height, float cellSize);

    bool loadHeightmapASC(const std::string &filename);

    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

private:
    Shader &shader;
    glm::mat4 model{1.0f};

    int width;
    int height;
    float cellSize;
    // include in read me

    float cellScale = 5.0f; // vertical exaggeration

    int meshWidth = 0;
    int meshHeight = 0;

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
