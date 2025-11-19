#include "Terrain.h"
#include "../util/debug.h"
#include <cassert>

// OUTLINE / TODOS

// Constructor:
// Save shader reference
// Save dimensions, size, position
// Set model matrix to identity
// Call helper function to create cubes and position them

// Constructor
Terrain::Terrain(Shader &shader, int width, int height, float cellSize)
{
    this->shader = shader;
    this->width = width;
    this->height = height;
    this->cellSize = cellSize;

    // Identity matrix
    this->model = glm::mat4(1.0f);

    // Initialize tiles
    this->initVAO();
    this->initVBO();
    this->initEBO();

    this->createTiles();
}

// Helper functions:
// Clear tiles vector
// Prepare cube colors
// Loop over x from 0 to width
// Compute worldX, worldZ, based on cellSize
// height = 0 for now, then we adjust later to elevation
// Position = (worldX, height, worldZ)
// Size = scaled cube dims
// Create Cube with shader, position, size, colors
// Add Cube to tiles vector

void Terrain::createTiles()
{
    vertices.clear();
    indices.clear();
    // Im confused on this vector? i think i am just missing something
    vector<color> colors =
        {
            // Front top right
            {1.0f, 0.0f, 0.0f},
            // Front top left
            {0.0f, 1.0f, 0.0f},
            // Front bottom right
            {0.0f, 0.0f, 1.0f},
            // Front bottom left
            {1.0f, 1.0f, 0.0f},
            // Back top right
            {1.0f, 0.0f, 1.0f},
            // Back top left
            {0.0f, 1.0f, 1.0f},
            // Back bottom right
            {0.5f, 0.5f, 0.5f},
            // Back bottom left
            {1.0f, 1.0f, 1.0f}};
    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < height; ++z)
        {
            float worldX = x * cellSize;
            float worldZ = z * cellSize;
            // Set to 0 for now to get it running
            // Will adjust later based on elevation
            float height = 0.0f;

            /*
            vertices.push_back(worldX);
            vertices.push_back(height);
            vertices.push_back(worldZ);
            #setup colors for each vertex and can add height data later
            vertices.push_back(colors[0].red);
             vertices.push_back(colors[0].green);
             vertices.push_back(colors[0].blue);

            */

            glm::vec3 pos = glm::vec3(worldX, height, worldZ);
            // Flat cube
            glm::vec3 size = glm::vec3(cellSize / 2.0f, cellSize / 10.0f, cellSize / 2.0f);

            Cube cube(this->shader, pos, size, colors);
            tiles.push_back(cube);
        }
    }
    for (int i = 0; i < height - 1; ++i)
    {
        for (j = 0; j < width - 1; ++j)
        {
            int topLeft = i * width + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * width + j;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}
void Terrain::draw(const glm::mat4 &view, const glm::mat4 &projection) const

{
    shader.use();
    shader.setMatrix4("model", this->model);
    shader.setMatrix4("view", this->view);
    shader.setMatrix4("projection", this->projection);

    glBlindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
