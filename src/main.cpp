#include <iostream>
#include <memory>
#include "Renderer.h"
#include "Scene.h"

int main() {

    try {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(scene, 800, 600);
        while (renderer->Run()) {

        }
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}