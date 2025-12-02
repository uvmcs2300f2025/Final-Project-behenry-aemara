#include "map/Terrain.h" // adjust path if needed (e.g., "Terrain.h" if in same folder)

#include <fstream>
#include <iostream>

#include <glad/glad.h>

// Constructor just stores basic info; real mesh is built in loadHeightmapASC
Terrain::Terrain(Shader &shader, int width, int height, float cellSize)
    : shader(shader),
      model(1.0f),
      width(width),
      height(height),
      cellSize(cellSize) {}

// Load an ESRI ASCII grid (.asc) heightmap
bool Terrain::loadHeightmapASC(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open heightmap file: " << filename << std::endl;
        return false;
    }

    std::string tag;
    int ncols = 0, nrows = 0;
    double xllcorner = 0.0, yllcorner = 0.0;
    double cellsize = 0.0;
    double nodata = -9999.0;

    file >> tag >> ncols;
    file >> tag >> nrows;
    file >> tag >> xllcorner;
    file >> tag >> yllcorner;
    file >> tag >> cellsize;
    file >> tag >> nodata;

    width = ncols;
    height = nrows;
    cellSize = static_cast<float>(cellsize);

    heightData.assign(height, std::vector<float>(width));

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            double h;
            if (!(file >> h))
            {
                std::cerr << "Unexpected end of file while reading height data\n";
                return false;
            }
            if (h == nodata)
            {
                h = 0.0;
            }
            heightData[row][col] = static_cast<float>(h);
        }
    }

    file.close();

    generateMesh();
    initBuffers();

    return true;
}

// Build a regular grid mesh out of heightData
void Terrain::generateMesh()
{
    vertices.clear();
    indices.clear();

    vertices.reserve(static_cast<size_t>(width) * height * 3);

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            float x = col * cellSize;
            float z = row * cellSize;
            float y = heightScale * heightData[row][col];

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int row = 0; row < height - 1; ++row)
    {
        for (int col = 0; col < width - 1; ++col)
        {
            unsigned int topLeft = row * width + col;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (row + 1) * width + col;
            unsigned int bottomRight = bottomLeft + 1;

            // Triangle 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triangle 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

// Upload mesh to GPU
void Terrain::initBuffers()
{
    if (VAO == 0)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                 indices.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, // location 0 in your vertex shader
        3, // x, y, z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float), // stride
        (void *)0);

    glBindVertexArray(0);
}

// Draw terrain
void Terrain::draw(const glm::mat4 &view, const glm::mat4 &projection) const
{
    if (VAO == 0 || indices.empty())
    {
        return;
    }

    shader.use();
    shader.setMatrix4("model", model);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
