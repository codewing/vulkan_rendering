#include <iostream>
#include <memory>
#include "Vulkan/Renderer.h"
#include "GLFWWindow.h"
#include "Scene.h"
#include "Time.h"

int main() {

    std::unique_ptr<Time> time = std::make_unique<Time>();
    std::shared_ptr<GLFWWindow> window = std::make_shared<GLFWWindow>("Vulkan Window", 1024, 768);
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(std::static_pointer_cast<Window>(window));
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(renderer);
    
    try {
        scene->Setup();
        do {
            time->UpdateTime();

            float aspect = window->Width()/static_cast<float>(window->Height());
            scene->UpdateCamera(time->GetTimeSinceStart(), aspect);

        } while (renderer->Run());

        scene->Teardown();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}