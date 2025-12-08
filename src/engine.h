#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader/shader.h"
#include "shader/shaderManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class Terrain;

struct HoverInfo
{
  bool valid = false;
  float worldX = 0.0f;
  float worldZ = 0.0f;
  float elevation = 0.0f;
};

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

  // mouse callback
  void onMouseMove(double xpos, double ypos);

private:
  unsigned int initWindow(bool debug = false);
  void initShaders();
  void initMatrices();
  void updateHoverElevation();

  GLFWwindow *window{nullptr};
  std::unique_ptr<Terrain> terrain;

  const unsigned int width = 800;
  const unsigned int height = 600;

  bool keys[1024]{};

  ShaderManager shaderManager;

  glm::mat4 view;
  glm::mat4 projection;

  float cameraZ;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  float camYaw = 0.0f;
  float camPitch = 45.0f;
  float camRadius = 600.0f;

  // mouse + hover
  double mouseX = 0.0;
  double mouseY = 0.0;
  HoverInfo hoverInfo;
};

#endif // ENGINE_H
