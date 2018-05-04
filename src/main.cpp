#include <iostream>
#include <memory>
#include "Renderer.h"
#include "Scene.h"
#include "Time.h"

int main() {

    try {
        std::unique_ptr<Time> time = std::make_unique<Time>();
        std::shared_ptr<Scene> scene = std::make_shared<Scene>();
        std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(scene, 800, 600);
        do {
            time->UpdateTime();

            scene->UpdateCamera(time->GetTimeSinceStart());

        } while (renderer->Run());
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}