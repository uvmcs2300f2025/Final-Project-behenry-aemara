#include "engine.h"
#include "map/Terrain.h"
#include <glm/gtc/matrix_transform.hpp>

// OUTLINE / TODOS
// Engine::render()
//  - Clear the screen
//  - Compute view and projection matrices
//  - shader.use()
//  - Call terrain.draw(view, projection)
//  - Swap buffers

Engine::Engine() : keys(), cameraZ(-3.0f)
{
  this->initWindow();
  this->initShaders();
  this->initMatrices();
}

Engine::~Engine()
{
  if (terrain)
  {
    delete terrain;
    terrain = nullptr;
  }
}

unsigned int Engine::initWindow(bool debug)
{
  // glfw: initialize and configure
  if (!glfwInit())
  {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
  if (!window)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // OpenGL configuration
  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSwapInterval(1); // vsync

  return 0;
}

void Engine::initShaders()
{
  // load shader manager
  shaderManager = ShaderManager();

  // Terrain shader
  terrainShader = shaderManager.loadShader(
      "../res/shaders/terrain.vert",
      "../res/shaders/terrain.frag",
      nullptr,
      "terrain");

  // Create terrain object
  terrain = new Terrain(terrainShader, 0, 0, 30.0f);

  // Load ASC heightmap
  if (!terrain->loadHeightmapASC("../res/heightmaps/rastert_dem_241.asc"))
  {
    std::cerr << "Failed to load heightmap" << std::endl;
  }

  // Vertical exaggeration
  terrain->setHeightScale(3.0f);
}

void Engine::initMatrices()
{
  // Camera / view matrix
  view = glm::lookAt(
      glm::vec3(0.0f, 0.0f, 3.0f),  // camera position
      glm::vec3(0.0f, 0.0f, 0.0f),  // look-at target
      glm::vec3(0.0f, 1.0f, 0.0f)); // up direction

  // Projection matrix (perspective)
  projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height),
      0.1f,
      100.0f);

  modelLeft = glm::mat4(1.0f);
  modelRight = glm::mat4(1.0f);
}

void Engine::processInput()
{
  glfwPollEvents();

  // Set keys to true if pressed, false if released
  for (int key = 0; key < 1024; ++key)
  {
    if (glfwGetKey(window, key) == GLFW_PRESS)
      keys[key] = true;
    else if (glfwGetKey(window, key) == GLFW_RELEASE)
      keys[key] = false;
  }

  // (optional) keyboard controls for cameraZ later if you want:
  // if (keys[GLFW_KEY_W]) cameraZ += 0.05f;
  // if (keys[GLFW_KEY_S]) cameraZ -= 0.05f;
}

void Engine::update()
{
  // Calculate delta time
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Engine::render()
{
  // Clear the screen before rendering the frame
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset view, then translate camera back along Z
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));

  if (terrain)
  {
    terrain->draw(view, projection);
  }

  glfwSwapBuffers(window);
}

bool Engine::shouldClose()
{
  return glfwWindowShouldClose(window);
}
