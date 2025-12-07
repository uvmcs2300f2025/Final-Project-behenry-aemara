#include "Terrain.h" // adjust path if needed (e.g., "Terrain.h" if in same folder)

#include <fstream>
#include <iostream>

#include <glad/glad.h>

// Constructor just stores basic info; real mesh is built in loadHeightmapASC
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

    // RESET min/max for this load
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

    // Safety check
    if (heightData.empty() || heightData[0].empty())
    {
        std::cerr << "generateMesh: no height data\n";
        return;
    }

    const int fullH = height;
    const int fullW = width;
    // Horizontal scale so terrain is ~10x10 units in X/Z
    xyScale = 0.005f;

    // Downsample factor – larger = fewer vertices
    const int sampleStep = 20;

    meshHeight = (fullH + sampleStep - 1) / sampleStep;
    meshWidth = (fullW + sampleStep - 1) / sampleStep;

    vertices.reserve(static_cast<size_t>(meshWidth) * meshHeight * 3);

    // Horizontal scale so terrain is ~10x10 units in X/Z

    // Vertical scale: how tall the whole terrain should be
    const float heightScale = 5.0f; // tweak this: 1–8 to taste

    // Center around (0,0) in X/Z
    const float halfW = static_cast<float>(fullW) / 2.0f;
    const float halfH = static_cast<float>(fullH) / 2.0f;

    // --- NEW: compute local min/max over the sampled points ---
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
        range = 1.0f; // avoid divide-by-zero

    // --- Build the vertex array using normalized heights ---
    for (int row = 0; row < fullH; row += sampleStep)
    {
        for (int col = 0; col < fullW; col += sampleStep)
        {
            // X/Z coordinates, centered
            float x = (static_cast<float>(col) - halfW) * xyScale;
            float z = (static_cast<float>(row) - halfH) * xyScale;

            // Height from DEM
            float h = heightData[row][col];

            // Normalize: t in [0,1], then shift to [-0.5,0.5]
            float t = (h - localMin) / range;
            float y = heightScale * (t - 0.5f);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    // Build index buffer (two triangles per quad)
    for (int r = 0; r < meshHeight - 1; ++r)
    {
        for (int c = 0; c < meshWidth - 1; ++c)
        {
            int tl = r * meshWidth + c;
            int tr = tl + 1;
            int bl = (r + 1) * meshWidth + c;
            int br = bl + 1;

            // Triangle 1
            indices.push_back(tl);
            indices.push_back(bl);
            indices.push_back(tr);

            // Triangle 2
            indices.push_back(tr);
            indices.push_back(bl);
            indices.push_back(br);
        }
    }

    std::cout << "Mesh (ASC): " << meshWidth << " x " << meshHeight
              << " verts=" << vertices.size() / 3
              << " tris=" << indices.size() / 3 << std::endl;
    std::cout << "Local min/max used in mesh: " << localMin
              << " / " << localMax << std::endl;
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

    float halfW = static_cast<float>(width) / 2.0f;
    float halfH = static_cast<float>(height) / 2.0f;

    // invert how we built the mesh:
    // x = (col - halfW) * xyScale  -> col = x/xyScale + halfW
    // z = (row - halfH) * xyScale  -> row = z/xyScale + halfH
    float colF = worldPos.x / xyScale + halfW;
    float rowF = worldPos.z / xyScale + halfH;

    int col = static_cast<int>(std::round(colF));
    int row = static_cast<int>(std::round(rowF));

    if (row < 0 || row >= height || col < 0 || col >= width)
        return false;

    outHeight = heightData[row][col];
    return true;
}
