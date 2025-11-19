#include "engine.h"

#include <iostream>


// OUTLINE / TODOS
// Create Engine
// Call engine.run()
// The Enginer will handle the. window loop and rendering


int main(int argc, char *argv[]) {
    Engine engine;

    while (!engine.shouldClose()) {
        engine.processInput();
        engine.update();
        engine.render();
    }

    glfwTerminate();
    return 0;
}
