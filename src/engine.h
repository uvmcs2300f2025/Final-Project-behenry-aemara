#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader/shader.h"
#include "shader/shaderManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <vector>

#include "map/Terrain.h"

using glm::lookAt;
using glm::mat4;
using glm::vec3;
using std::make_unique;
using std::unique_ptr;
using std::vector;

class Engine
{
public:
  Engine();
  ~Engine();

  bool init();
  void run();

  bool shouldClose();
  void processInput();
  void update();
  void render();

private:
  GLFWwindow *window{nullptr};
  const unsigned int width = 800;
  const unsigned int height = 600;

  bool keys[1024]{};

  ShaderManager shaderManager;

  glm::mat4 modelLeft;
  glm::mat4 modelRight;
  glm::mat4 view;
  glm::mat4 projection;

  float cameraZ;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // Terrain object 
  unique_ptr<Terrain> terrain;

  unsigned int initWindow(bool debug = false);
  void initShaders();
  void initMatrices();
};

#endif // ENGINE_H
