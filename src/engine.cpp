#include "engine.h"
#include <iostream>

Engine::Engine() : cameraZ(-3.0f)
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

Engine::~Engine() {}

// create the GLFW window and OpenGL context
unsigned int Engine::initWindow(bool debug)
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
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
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSwapInterval(1);

  return 0;
}

void Engine::initShaders()
{
  shaderManager = ShaderManager();
  // later we’ll load shaders here
}

void Engine::initMatrices()
{
  view = lookAt(vec3(0.0f, 0.0f, 3.0f),
                vec3(0.0f, 0.0f, 0.0f),
                vec3(0.0f, 1.0f, 0.0f));

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

  for (int key = 0; key < 1024; ++key)
  {
    if (glfwGetKey(window, key) == GLFW_PRESS)
      keys[key] = true;
    else if (glfwGetKey(window, key) == GLFW_RELEASE)
      keys[key] = false;
  }

  // add key handling here later if you want
}

void Engine::update()
{
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Engine::render()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset view each frame and move camera back
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));

  // later we’ll call terrain.draw(view, projection) here

  // nothing drawn yet → black screen is expected
}

bool Engine::shouldClose()
{
  return glfwWindowShouldClose(window);
}

// --------- the two missing functions ---------

bool Engine::init()
{
  // constructor already called initWindow/initShaders/initMatrices
  if (!window)
  {
    std::cerr << "Engine::init() - window is null, initWindow must have failed\n";
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
