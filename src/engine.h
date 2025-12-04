#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader/shader.h"
// #include "map/build/Terrain.h"

#include <iostream>
#include <memory>
#include <vector>

// #include "src/shapes/cube.h"
#include "shader/shaderManager.h"

// Engine owns a terrain, which it initalizes in the constructor
// The render methods will call terrain.draw() each frame

using std::vector, std::unique_ptr, std::make_unique, glm::ortho, glm::cross,
    glm::mat4, glm::vec3, glm::vec4;

/**
 * @brief The Engine class.
 * @details The Engine class is responsible for initializing the GLFW window,
 * loading shaders, and rendering the game state.
 */
class Engine
{
private:
  /// @brief The actual GLFW window.
  GLFWwindow *window{};

  /// @brief The width and height of the window.
  const unsigned int width = 800, height = 600; // Window dimensions

  /// @brief Keyboard state (True if pressed, false if not pressed).
  /// @details Index this array with GLFW_KEY_{key} to get the state of a key.
  bool keys[1024];

  /// @brief Responsible for loading and storing all the shaders used in the
  /// project.
  /// @details Initialized in initShaders()
  ShaderManager shaderManager;

  // Transformation matrix
  glm::mat4 modelLeft; // Model matrix for a 3D object
  glm::mat4 modelRight;
  glm::mat4 view;       // The camera's position and orientation in the world
  glm::mat4 projection; // Orthographic projection matrix maps a 3D scene to a
                        // 2D viewport

  // Shapes
  // unique_ptr<Cube> cubeLeft;
  // unique_ptr<Cube> cubeRight;

  // Keep track of the camera's distance from the origin
  // Moving the camera closer and farther will have the
  // visual effect of making the cube larger and smaller
  float cameraZ;

  // Shaders
  // Shader cubeShader;

  /// @note Call glCheckError() after every OpenGL call to check for errors.
  GLenum glCheckError_(const char *file, int line);
/// @brief Macro for glCheckError_ function. Used for debugging.
#define glCheckError() glCheckError_(__FILE__, __LINE__)

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
  GLFWwindow *window;
  int width = 800;
  int height = 600;
  float cameraZ;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  unsigned int initWindow(bool debug = false);

  void initShaders();
  void initShapes();

  void processInput();
  void initMatrices();

  void update();
  void render();
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  bool shouldClose();
};

#endif // GRAPHICS_ENGINE_H
