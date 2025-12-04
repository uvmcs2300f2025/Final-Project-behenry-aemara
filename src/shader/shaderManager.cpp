#include "shaderManager.h"
#include <fstream>
#include <sstream>

ShaderManager::~ShaderManager()
{
    clear();
}

Shader ShaderManager::loadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    return shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

Shader &ShaderManager::getShader(std::string name)
{
    return shaders[name];
}

void ShaderManager::clear()
{
    // delete all shaders: "iter" here is const std::pair<std::string, Shader>&, so we need to use
    // "iter.second" to get the Shader, and delete the program by ID
    for (const auto &iter : shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ShaderManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    // open files
    auto loadFile = [&](const char *filePath) -> std::string
    {
        if (!filePath)
            return "";
        std::ifstream file;
        std::stringstream stream;
        try
        {
            file.open(filePath);
            if (!file.is_open())
            {
                throw std::runtime_error("Could not open file");
            }
            stream << file.rdbuf();
            file.close();
            std::string code = stream.str();
            std::cout << "\n\n--- DEBUG: Loaded Source for " << filePath << " ---\n"
                      << (code.size() > 200 ? code.substr(0, 200) + "...\n(TRUNCATED)\n" : code)
                      << "----------------------------------------\n";
            // --------------------------------------------------------
            return code;
        }
        catch (std::exception &e)
        {
            std::cout << "ERROR::SHADER: Failed to read shader file: " << filePath << "\n"
                      << e.what() << std::endl;
            return "";
        }
    };
    // Load files using the helper
    vertexCode = loadFile(vShaderFile);
    fragmentCode = loadFile(fShaderFile);
    geometryCode = loadFile(gShaderFile);

    // CRITICAL CHECK: If file loading failed, don't attempt to compile
    if (vertexCode.empty() || fragmentCode.empty())
    {
        std::cerr << "!!! FATAL ERROR: Cannot compile shader due to empty source code." << std::endl;
        // Optionally, return a dummy shader ID to prevent crashing elsewhere
        Shader dummy;
        dummy.ID = 0;
        return dummy;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.empty() ? nullptr : geometryCode.c_str();

    // 2. now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderCode);
    return shader;
}