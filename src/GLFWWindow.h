//
// Created by codewing on 20/10/2017.
//

#pragma once

#include "Window.h"

struct GLFWwindow;

class GLFWWindow : public Window {

private:
    GLFWwindow *window = nullptr;

    virtual void Initialize() override;

    virtual void CleanUp() override;

    static void FramebufferResizedCB(GLFWwindow *window, int width, int height);

public:
    GLFWWindow(std::string name, int width, int height);

    virtual ~GLFWWindow();

    virtual bool Update() override;
    bool CreateSurface(VkInstance instance, VkSurfaceKHR* surface);

    GLFWwindow *GetGLFWwindow();
};
