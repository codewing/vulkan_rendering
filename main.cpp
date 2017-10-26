#include <iostream>
#include <memory>
#include "Renderer.h"

int main() {

    try {
        std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(800, 600);
        while(renderer->Run()){

        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}