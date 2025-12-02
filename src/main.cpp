#include "engine.h"
#include "map/Terrain.h"
#include <GLFW/glfw3.h>

#include <iostream>

// OUTLINE / TODOS
// Create Engine
// Call engine.run()
// The Enginer will handle the. window loop and rendering

int main(int argc, char *argv[])
{
    Engine engine;

    while (!engine.shouldClose())
    {
        engine.processInput();
        engine.update();
        engine.render();
    }
    // stop program after test

    glfwTerminate();
    return 0;
}
