#ifndef GRAPHICS_CUBE_H
#define GRAPHICS_CUBE_H

#include / Users / annemara / Desktop / FP2300 / Final - Project - behenry - aemara / src / shader / shader.h

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector, glm::vec3, glm::mat4;

// OUTLINE / TODOS

// 1. Store a reference to the shader used to draw the cube
// 2. Store terrain dimensions and position
// 3. Store a vector of Cube objects
// 4. Build a grid of cubes - flat for now until it works, then add 3D modeling later
// 5. Draw all cubes using the shader








class Cube
{
public:
    Cube(Shader &shader, vec3 pos, vec3 size, vector<color> colors);
    ~Cube();
    void draw(const mat4 &model, const mat4 &view, const mat4 &projection) const;
    void setUniforms(const mat4 &model, const mat4 &view, const mat4 &projection) const;

    void rotateX(float delta);
    void rotateY(float delta);
    void rotateZ(float delta);

    void changeSize(float delta);

private:
    Shader shader;
    vec3 pos;
    vec3 size;
    vector<color> colors;
    float degreeX;
    float degreeY;
    float degreeZ;

    // Vectors
    vector<float> vertices;
    vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void initVectors();
    void initVAO();
    void initVBO();
    void initEBO();
};

#endif // GRAPHICS_CUBE_H