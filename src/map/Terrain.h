#ifndef TERRAIN_H
#define TERRAIN_H

#include "../shader/shader.h"

#include <vector>
#include <limits>
#include <string>

#include <glm/glm.hpp>
class Terrain
{
public:
    Terrain(Shader &shader, int width, int height, float cellSize);
    bool worldToHeight(const glm::vec3 &worldPos, float &outHeight) const;

    bool loadHeightmapASC(const std::string &filename);

    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

private:
    Shader &shader;
    glm::mat4 model{1.0f};
    float xyScale = 0.005f; // must match the horizontal scale value

    int width;
    int height;
    float cellSize;
    // you can chnage the cellscale, but will have to adjust the camera height in conjuction
    float cellScale = .02f; // vertical exaggeration
    float heightScale = 5.0f;

    int meshWidth = 0;
    int meshHeight = 0;

    std::vector<std::vector<float>> heightData;

    // Meshing my perfect data using 3 floats per vertex
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float minHeight = std::numeric_limits<float>::infinity();
    float maxHeight = -std::numeric_limits<float>::infinity();

    // OpenGL buffer objects - just like standard
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void generateMesh(); // build vertices & indices from heightData
    void initBuffers();  // upload data to OpenGL buffers friends with the unsigned above
};

#endif // TERRAIN_H
