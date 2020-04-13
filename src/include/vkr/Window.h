//
// Created by codewing on 11/03/2020.
//

#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <functional>

class Window {

protected:

    int width, height;
    std::string name;
    bool running;

    virtual void CleanUp() {};
    virtual void Initialize() {};

public:
    // Bound by the renderer
    std::function<void()> OnResizedEvent;

    Window(std::string name, int width, int height);
    virtual ~Window();

    virtual bool Update() = 0;
    virtual bool CreateSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;

    [[nodiscard]]
    int Width() const { return width; };
    [[nodiscard]]
    int Height() const { return height; };

};