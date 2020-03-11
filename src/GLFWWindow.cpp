//
// Created by codewing on 20/10/2017.
//

#include "GLFWWindow.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>

GLFWWindow::GLFWWindow(std::string name, int width, int height) : Window(name, width, height) {
    running = true;

    Initialize();
}

GLFWWindow::~GLFWWindow() { }

void GLFWWindow::Initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //Don't create a OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, GLFWWindow::FramebufferResizedCB);
}

bool GLFWWindow::Update() {
    if (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    } else {
        running = false;
    }
    return running;
}

void GLFWWindow::FramebufferResizedCB(GLFWwindow *window, int width, int height) {
    GLFWWindow *currentWindow = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
    currentWindow->OnResizedEvent();
}

void GLFWWindow::CleanUp() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

GLFWwindow *GLFWWindow::GetGLFWwindow() {
    return window;
}

bool GLFWWindow::CreateSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    return true;
}
