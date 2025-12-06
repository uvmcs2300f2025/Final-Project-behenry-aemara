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

    bool loadHeightmapASC(const std::string &filename);

    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

private:
    Shader &shader;
    glm::mat4 model{1.0f};

    int width;
    int height;
    float cellSize;
    // include in read me
    /// okay so cell scale is vertical exaggeration,
    /// and the split screen came when this value was 5
    ////gonna drop it down to .5 and see if that helps
    //  when i just chnaged this it made no dif
    /// GOING to move the amera placenet higher and see if that chnages things
    float cellScale = .02f;    // vertical exaggeration
    float heightScale = 15.0f; // or whatever you’re using
    float xyScale = 0.005f;    // if you want this as a member too

    int meshWidth = 0;
    int meshHeight = 0;

    std::vector<std::vector<float>> heightData;

    // Mesh data: 3 floats per vertex (x, y, z)
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float minHeight = std::numeric_limits<float>::infinity();
    float maxHeight = -std::numeric_limits<float>::infinity();

    // OpenGL buffer objects
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    void generateMesh(); // build vertices/indices from heightData
    void initBuffers();  // upload data to OpenGL buffers
};

#endif // TERRAIN_H
