#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "shader/shaderManager.h"
#include "map/Terrain.h"

class Engine
{
public:
  Engine();
  ~Engine();

  // main loop helpers
  void processInput();
  void update();
  void render();
  bool shouldClose();

private:
  // window
  GLFWwindow *window = nullptr;
  const unsigned int width = 800;
  const unsigned int height = 600;

  // input
  bool keys[1024]{};

  // shaders / terrain
  ShaderManager shaderManager;
  Shader terrainShader;
  Terrain *terrain = nullptr;

  // timing
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // camera / matrices
  float cameraZ;
  glm::mat4 view;
  glm::mat4 projection;
  glm::mat4 modelLeft;
  glm::mat4 modelRight;

  // init helpers
  unsigned int initWindow(bool debug = false);
  void initShaders();
  void initMatrices();
};

#endif // ENGINE_H
