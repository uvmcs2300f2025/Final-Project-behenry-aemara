#include "Terrain.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#include <glad/glad.h>

// Constructor
Terrain::Terrain(Shader &shader, int width, int height, float cellSize)
    : shader(shader),
      model(1.0f),
      width(width),
      height(height),
      cellSize(cellSize)
{
}
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

    heightData.assign(height, std::vector<float>(width));

    // resets the  min & max for this load
    minHeight = std::numeric_limits<float>::infinity();
    maxHeight = -std::numeric_limits<float>::infinity();

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

            float hf = static_cast<float>(h);
            heightData[row][col] = hf;

            if (hf < minHeight)
                minHeight = hf;
            if (hf > maxHeight)
                maxHeight = hf;
        }
    }

    file.close();

    std::cout << "ASC loaded: " << width << " x " << height << std::endl;
    std::cout << "minHeight = " << minHeight
              << " maxHeight = " << maxHeight << std::endl;

    generateMesh();
    initBuffers();

    return true;
}

void Terrain::generateMesh()
{
    vertices.clear();
    indices.clear();

    if (heightData.empty() || heightData[0].empty())
    {
        std::cerr << "generateMesh: no height data\n";
        return;
    }

    const int fullH = height;
    const int fullW = width;

    // Downsample factor: when larger is has fewer vertices
    // if being run on an actual computer you can make the value one for max detail
    const int sampleStep = 20;

    meshHeight = (fullH + sampleStep - 1) / sampleStep;
    meshWidth = (fullW + sampleStep - 1) / sampleStep;

    vertices.reserve(static_cast<size_t>(meshWidth) * meshHeight * 3);

    // matching this data to worldToHeight values
    const float scaleXY = xyScale;    // 0.005f from header
    const float vScale = heightScale; // vertical exaggeration

    const float halfW = static_cast<float>(fullW) / 2.0f;
    const float halfH = static_cast<float>(fullH) / 2.0f;

    // local min and max for data normilization
    float localMin = std::numeric_limits<float>::infinity();
    float localMax = -std::numeric_limits<float>::infinity();

    for (int row = 0; row < fullH; row += sampleStep)
    {
        for (int col = 0; col < fullW; col += sampleStep)
        {
            float h = heightData[row][col];
            if (h < localMin)
                localMin = h;
            if (h > localMax)
                localMax = h;
        }
    }

    float range = localMax - localMin;
    if (range <= 0.0001f)
        range = 1.0f;

    // vertices
    for (int row = 0; row < fullH; row += sampleStep)
    {
        for (int col = 0; col < fullW; col += sampleStep)
        {
            float x = (static_cast<float>(col) - halfW) * scaleXY;
            float z = (static_cast<float>(row) - halfH) * scaleXY;

            float h = heightData[row][col];
            float t = (h - localMin) / range; // 0..1
            float y = vScale * (t - 0.5f);    // centered around 0

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    // Indices: using two triangles per quad
    for (int r = 0; r < meshHeight - 1; ++r)
    {
        for (int c = 0; c < meshWidth - 1; ++c)
        {
            int tl = r * meshWidth + c;
            int tr = tl + 1;
            int bl = (r + 1) * meshWidth + c;
            int br = bl + 1;

            indices.push_back(tl);
            indices.push_back(bl);
            indices.push_back(tr);

            indices.push_back(tr);
            indices.push_back(bl);
            indices.push_back(br);
        }
    }

    std::cout << "Mesh (ASC): " << meshWidth << " x " << meshHeight
              << " verts=" << vertices.size() / 3
              << " tris=" << indices.size() / 3 << std::endl;
    std::cout << "Local min/max used in mesh: "
              << localMin << " / " << localMax << std::endl;
}

// Upload mesh to GPU
// this is how the map is constructed
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
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *)0);

    glBindVertexArray(0);
}

void Terrain::draw(const glm::mat4 &view, const glm::mat4 &projection) const
{
    shader.use();

    glm::mat4 I(1.0f);
    shader.setMatrix4("model", I);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    shader.setFloat("uHeightScale", heightScale);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}
bool Terrain::worldToHeight(const glm::vec3 &worldPos, float &outHeight) const
{
    if (heightData.empty() || heightData[0].empty())
        return false;

    const float fullW = static_cast<float>(width);
    const float fullH = static_cast<float>(height);
    const float halfW = fullW / 2.0f;
    const float halfH = fullH / 2.0f;

    // gets inverted to get the fractional col/row
    float colF = worldPos.x / xyScale + halfW;
    float rowF = worldPos.z / xyScale + halfH;

    if (colF < 0.0f || colF > fullW - 1.0f ||
        rowF < 0.0f || rowF > fullH - 1.0f)
    {
        return false;
    }

    // Bilinear interpolation
    int c0 = static_cast<int>(floorf(colF));
    int r0 = static_cast<int>(floorf(rowF));
    int c1 = std::min(c0 + 1, width - 1);
    int r1 = std::min(r0 + 1, height - 1);

    float tx = colF - static_cast<float>(c0);
    float ty = rowF - static_cast<float>(r0);

    float h00 = heightData[r0][c0];
    float h10 = heightData[r0][c1];
    float h01 = heightData[r1][c0];
    float h11 = heightData[r1][c1];

    float h0 = h00 + tx * (h10 - h00);
    float h1 = h01 + tx * (h11 - h01);
    outHeight = h0 + ty * (h1 - h0); // HOVER FUNCTIONNNNNNNN

    return true;
}
