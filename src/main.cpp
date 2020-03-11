#include <iostream>
#include <memory>
#include "Vulkan/Renderer.h"
#include "GLFWWindow.h"
#include "Scene.h"
#include "Time.h"

int main() {

    std::unique_ptr<Time> time = std::make_unique<Time>();
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::shared_ptr<GLFWWindow> window = std::make_shared<GLFWWindow>("Vulkan Window", 1024, 768);
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(scene, std::static_pointer_cast<Window>(window));

    try {
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