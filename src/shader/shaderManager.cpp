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

    for (const auto &iter : shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ShaderManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{

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

            return code;
        }
        catch (std::exception &e)
        {
            std::cout << "ERROR::SHADER: Failed to read shader file: " << filePath << "\n"
                      << e.what() << std::endl;
            return "";
        }
    };
    // Load files
    vertexCode = loadFile(vShaderFile);
    fragmentCode = loadFile(fShaderFile);
    geometryCode = loadFile(gShaderFile);

    // safety check to ensure the file loads
    if (vertexCode.empty() || fragmentCode.empty())
    {
        std::cerr << "!!! FATAL ERROR: Cannot compile shader due to empty source code." << std::endl;
        // this is additional safety to prevent crashes
        Shader dummy;
        dummy.ID = 0;
        return dummy;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.empty() ? nullptr : geometryCode.c_str();

    // no creating shader object
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderCode);
    return shader;
}