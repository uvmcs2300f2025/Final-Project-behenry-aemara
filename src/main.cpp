#include "engine.h"
#include <iostream>

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

    // Engine owns the game loop
    engine.run();

    std::cout << "program ended normally\n";
    return 0;
}
