#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader/shader.h"
#include "shader/shaderManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <vector>
#include <iostream>

class Terrain;

class Engine
{
public:
  Engine();
  ~Engine();

  // this is for the hover function
  void onMouseMove(double xpos, double ypos);

  // set everything up (window, shaders, terrain, matrices)
  bool init();

  // main loop
  void run();

  // per-frame stuff
  bool shouldClose();
  void processInput();
  void update();
  void render();

private:
  // --- Window / GL state ---
  GLFWwindow *window{nullptr};
  const unsigned int width = 800;
  const unsigned int height = 600;

  bool keys[1024]{};

  // --- Scene objects ---
  std::unique_ptr<Terrain> terrain;
  ShaderManager shaderManager;
  double mouseX = 0.0;
  double mouseY = 0.0;
  // --- Matrices ---
  glm::mat4 view{1.0f};
  glm::mat4 projection{1.0f};

  // (you can delete these if you never use them)
  glm::mat4 modelLeft{1.0f};
  glm::mat4 modelRight{1.0f};

  // --- Timing ---
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // --- Orbit camera params ---
  float cameraZ = -200.0f;  // legacy, can remove if unused
  float camYaw = 0.0f;      // left/right angle in degrees
  float camPitch = -45.0f;  // up/down tilt in degrees
  float camRadius = 800.0f; // distance from center

  // --- Internal helpers ---
  unsigned int initWindow(bool debug = false);
  void initShaders();
  void initMatrices();
};

#endif // ENGINE_H
