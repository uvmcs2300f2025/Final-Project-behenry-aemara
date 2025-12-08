#include "engine.h"
#include <iostream>
#include "map/Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

Engine::Engine() : cameraZ(-200.0f)
{
  // initialize key array
  for (bool &k : keys)
  {
    k = false;
  }

  // set up window, shaders, matrices
  if (initWindow() != 0)
  {
    std::cerr << "Engine constructor: initWindow failed\n";
    return;
  }

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
  (void)debug; // unused for now

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

  // starting view
  glViewport(0, 0, width, height);

  // blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  // vsync on
  glfwSwapInterval(1);

  // THIS IS HOVER
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(
      window,
      [](GLFWwindow *win, double xpos, double ypos)
      {
        Engine *eng = static_cast<Engine *>(glfwGetWindowUserPointer(win));
        if (eng)
        {
          eng->onMouseMove(xpos, ypos);
        }
      });

  return 0;
}
void Engine::onMouseMove(double xpos, double ypos)
{
  mouseX = xpos;
  mouseY = ypos;
}

void Engine::initShaders()
{
  shaderManager = ShaderManager();

  shaderManager.loadShader(
      "../res/shaders/shape3D.vert",
      "../res/shaders/shape3D.frag",
      nullptr,
      "terrain");
}

void Engine::initMatrices()
{
  // setting up the camera view
  view = glm::lookAt(
      glm::vec3(0.0f, 60.0f, .1f), // eye position
      glm::vec3(0.0f, 0.0f, 0.0f), // keeping it centered
      glm::vec3(0.0f, 0.0f, -1.0f) // viewpoint
  );

  projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height),
      0.1f,
      5000.0f // this can be played with
  );
  camYaw = 0.0f;
  camPitch = 45.0f;
  camRadius = 600.0f;
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

  // killing the window
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  // spinny spinny camera
  float rotSpeed = 150.0f * deltaTime;  // speed
  float zoomSpeed = 250.0f * deltaTime; // speed but distance

  // lefty loosey righty tighty
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    camYaw -= rotSpeed;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camYaw += rotSpeed;

  // up down and around the world
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camPitch += rotSpeed;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camPitch -= rotSpeed;

  // Clamping not camping
  if (camPitch < 10.0f)
    camPitch = 10.0f; // min
  if (camPitch > 80.0f)
    camPitch = 80.0f; // max

  // ZOOOMMMING through the state
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camRadius -= zoomSpeed;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camRadius += zoomSpeed;

  if (camRadius < 25.0f)
    camRadius = 25.0f; // don't go inside VT - that would be unkind
  if (camRadius > 3000.0f)
    camRadius = 3000.0f; // don't go to space - that would also be unkind
}

void Engine::update()
{
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // moving angles to radians like any modern woman
  float yawRad = glm::radians(camYaw);
  float pitchRad = glm::radians(camPitch);

  // Spherical as sphere is world but world is flat
  float x = camRadius * cosf(pitchRad) * sinf(yawRad);
  float y = camRadius * sinf(pitchRad);
  float z = camRadius * cosf(pitchRad) * cosf(yawRad);

  // center of the terrain is the center of my heart
  view = glm::lookAt(
      glm::vec3(x, y, z),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
}
void Engine::updateHoverElevation()
{
  if (!terrain || !window)
    return;

  hoverInfo.valid = false;

  int mx = static_cast<int>(mouseX);
  int my = static_cast<int>(mouseY);

  // orgin smorgin
  int readY = static_cast<int>(height - my - 1);

  if (mx < 0 || mx >= static_cast<int>(width) ||
      readY < 0 || readY >= static_cast<int>(height))
  {
    return;
  }

  // reading the ellie baby
  float depth = 1.0f;
  glReadPixels(mx, readY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

  // BACKGROUND IS BACKGROUND not data
  if (depth == 1.0f)
    return;

  glm::vec3 winCoord(mouseX, readY, depth);
  glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));

  glm::mat4 model(1.0f); // terrain is terrain, dont change this from 1, it will break everything
  glm::mat4 mv = view * model;

  glm::vec3 worldPos = glm::unProject(winCoord, mv, projection, viewport);

  float elev = 0.0f;

  if (terrain->worldToHeight(worldPos, elev))
  {
    hoverInfo.valid = true;
    hoverInfo.worldX = worldPos.x;
    hoverInfo.worldZ = worldPos.z;
    hoverInfo.elevation = elev;

    // intresting stuff
    // this is the title bar once you start reading data which i think is cool
    // as it orig says final project but now its more fun
    std::ostringstream title;
    title << " Scaled Elevation Model of VT  |  Elevation: " << static_cast<int>(elev) << " m";
    glfwSetWindowTitle(window, title.str().c_str());
  }
}

void Engine::render()
{
  glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  // this is awesome wireframe stuff to see the mesh
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (terrain)
  {
    terrain->draw(view, projection);
  }
  updateHoverElevation();
  // this is teh friend of the commented line like 5 lines up awesome for debugging

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Engine::shouldClose()
{
  return glfwWindowShouldClose(window);
}

bool Engine::init()
{
  // bro dont call the instructor here
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

  if (!terrain->loadHeightmapASC("../res/heightmaps/rastert_dem_241.asc"))
  {
    std::cerr << "Failed to load heightmap ../res/heightmaps/rastert_dem_241.asc\n";
    return false;
  }

  std::cout << "engine.init() SUCCEEDED, entering loop\n";
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
