//
// Created by codewing on 20/10/2017.
//

#include "Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

Window::Window(int width, int height) : WINDOW_WIDTH(width), WINDOW_HEIGHT(height) {
    running = true;

    InitWindow();
}

Window::~Window() {
    CleanUp();
}

void Window::InitWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //Don't create a OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", nullptr, nullptr);
}

bool Window::Update() {
    if (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    } else {
        running = false;
    }
    return running;
}

void Window::CleanUp() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

GLFWwindow* Window::GetGLFWwindow() {
    return window;
}

const int Window::GetWidth() const {
    return WINDOW_WIDTH;
}

const int Window::GetHeight() const {
    return WINDOW_HEIGHT;
}
