#include "engine.h"
#include <iostream>
#include "map/Terrain.h"

Engine::Engine() : cameraZ(-200.0f)
{
  // initialize key array
  for (bool &k : keys)
  {
    k = false;
  }

  // set up window, shaders, matrices
  initWindow();
  initShaders();
  initMatrices();
}

Engine::~Engine()
{
  if (window)
  {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

// create the GLFW window and OpenGL context
unsigned int Engine::initWindow(bool debug)
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW\n";
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(width, height, "Final Project", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD\n";
    return 1;
  }

  // initial viewport
  glViewport(0, 0, width, height);

  // blending + depth
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  // vsync on
  glfwSwapInterval(1);

  return 0;
}

void Engine::initShaders()
{
  shaderManager = ShaderManager();
  shaderManager.loadShader("/Users/annemara/Desktop/Final-Project-behenry-aemara/res/shaders/shape3D.vert",
                           "/Users/annemara/Desktop/Final-Project-behenry-aemara/res/shaders/shape3D.frag",
                           nullptr,
                           "terrain");
  // later:
  // shaderManager.loadShader("terrain", "res/shaders/terrain.vert", "res/shaders/terrain.frag");
}

void Engine::initMatrices()
{
  // camera looking at origin
  view = lookAt(vec3(0.0f, 0.0f, 3.0f),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 1.0f, 0.0f));

  projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height),
      0.1f,
      5000.0f);

  modelLeft = glm::mat4(1.0f);
  modelRight = glm::mat4(1.0f);
}

void Engine::processInput()
{
  glfwPollEvents();

  for (int key = 0; key < 1024; ++key)
  {
    if (glfwGetKey(window, key) == GLFW_PRESS)
      keys[key] = true;
    else if (glfwGetKey(window, key) == GLFW_RELEASE)
      keys[key] = false;
  }

  // basic escape handling
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

void Engine::update()
{
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // later: move camera / animate things with deltaTime
}

void Engine::render()
{
  glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // reset view each frame and move camera back
  view = glm::mat4(1.0f);
  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));
  if (terrain)
  {
    terrain->draw(view, projection);
  }
  // later:
  // auto &shader = shaderManager.getShader("terrain");
  // terrain.draw(view, projection);

  // right now, we draw nothing → just a black window
}

bool Engine::shouldClose()
{
  return glfwWindowShouldClose(window);
}

bool Engine::init()
{
  // constructor already called initWindow/initShaders/initMatrices
  if (!window)
  {
    std::cerr << "Engine::init() - window is null, initWindow must have failed\n";
    return false;
  }
  Shader &terrainShader = shaderManager.getShader("terrain");
  int gridWidth = 500;
  int gridHeight = 500;
  float cellSize = 1.0f;

  terrain = std::make_unique<Terrain>(terrainShader, gridWidth, gridHeight, cellSize);

  // path is from the *build* directory → ../res/heightmaps/...
  if (!terrain->loadHeightmapASC("../res/heightmaps/rastert_dem_241.asc"))
  {
    std::cerr << "Failed to load heightmap ../res/heightmaps/rastert_dem_241.asc\n";
    return false;
  }
  return true;
}

void Engine::run()
{
  while (!shouldClose())
  {
    processInput();
    update();
    render();

    glfwSwapBuffers(window);
  }
}
