#include "engine.h"
#include <iostream>
#include "map/Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  // initial viewport
  glViewport(0, 0, width, height);

  // blending + depth
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);

  // vsync on
  glfwSwapInterval(1);

  // --- mouse tracking setup ---
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
      "/Users/annemara/Desktop/Final-Project-behenry-aemara/res/shaders/shape3D.vert",
      "/Users/annemara/Desktop/Final-Project-behenry-aemara/res/shaders/shape3D.frag",
      nullptr,
      "terrain");
}

void Engine::initMatrices()
{
  // Simple, known-good angled camera that used to work
  view = glm::lookAt(
      glm::vec3(0.0f, 60.0f, .1f), // eye position (back and above)
      glm::vec3(0.0f, 0.0f, 0.0f), // look at the origin
      glm::vec3(0.0f, 0.0f, -1.0f) // world up
  );

  projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(width) / static_cast<float>(height),
      0.1f,
      5000.0f // far plane large enough for big DEM
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

  // basic escape handling
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  // -------- NEW: orbit camera controls --------
  float rotSpeed = 150.0f * deltaTime;  // deg/sec
  float zoomSpeed = 250.0f * deltaTime; // units/sec

  // Left / Right → spin around VT
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    camYaw -= rotSpeed;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camYaw += rotSpeed;

  // Up / Down → tilt camera
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camPitch += rotSpeed;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camPitch -= rotSpeed;

  // Clamp pitch so we don't flip
  if (camPitch < 10.0f)
    camPitch = 10.0f; // minimum tilt
  if (camPitch > 80.0f)
    camPitch = 80.0f; // maximum tilt

  // Optional: zoom in/out with W/S
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camRadius -= zoomSpeed;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camRadius += zoomSpeed;

  if (camRadius < 50.0f)
    camRadius = 50.0f; // don't go inside VT
  if (camRadius > 3000.0f)
    camRadius = 3000.0f; // don't go to space
}

void Engine::update()
{
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  // Convert angles to radians
  float yawRad = glm::radians(camYaw);
  float pitchRad = glm::radians(camPitch);

  // Spherical coordinates → Cartesian (orbit around origin)
  float x = camRadius * cosf(pitchRad) * sinf(yawRad);
  float y = camRadius * sinf(pitchRad);
  float z = camRadius * cosf(pitchRad) * cosf(yawRad);

  // Look at center of terrain (assumed around origin)
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

  // OpenGL origin = bottom-left, GLFW mouse origin = top-left
  int readY = static_cast<int>(height - my - 1);

  if (mx < 0 || mx >= static_cast<int>(width) ||
      readY < 0 || readY >= static_cast<int>(height))
  {
    return;
  }

  // 1) Read depth under the mouse
  float depth = 1.0f;
  glReadPixels(mx, readY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

  // depth == 1.0 → nothing drawn there (background)
  if (depth == 1.0f)
    return;

  // 2) Unproject to world space
  glm::vec3 winCoord(mouseX, readY, depth);
  glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));

  glm::mat4 model(1.0f); // terrain uses identity model
  glm::mat4 mv = view * model;

  glm::vec3 worldPos = glm::unProject(winCoord, mv, projection, viewport);

  // 3) Map worldPos → DEM height
  float elev = 0.0f;
  if (terrain->worldToHeight(worldPos, elev))
  {
    hoverInfo.valid = true;
    hoverInfo.worldX = worldPos.x;
    hoverInfo.worldZ = worldPos.z;
    hoverInfo.elevation = elev;

    // For now: spam console so we see it works
    std::cout << "Hover elevation: " << elev << std::endl;
  }
}

void Engine::render()
{
  glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  // Uncomment to see wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (terrain)
  {
    terrain->draw(view, projection);
  }
  updateHoverElevation();

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

  // These initial values are mostly ignored once ASC is loaded
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
