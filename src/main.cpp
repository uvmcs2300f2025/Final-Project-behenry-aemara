#include "engine.h"
#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[])
{
    std::cout << "main started\n";

    Engine engine;

    // initialize window, OpenGL, shaders, terrain, etc.
    if (!engine.init())
    {
        std::cerr << "engine.init() FAILED\n";
        return -1;
    }

    std::cout << "engine.init() SUCCEEDED, entering loop\n";

    // let Engine own the while(!shouldClose) loop internally
    engine.run();

    std::cout << "program ended normally\n";

    glfwTerminate();
    return 0;
}
