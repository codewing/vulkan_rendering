//
// Created by codewing on 20/10/2017.
//

#include "Window.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

Window::Window(Renderer *renderer, int width, int height) : WINDOW_WIDTH(width), WINDOW_HEIGHT(height) {
    running = true;
    this->renderer = renderer;

    InitWindow();
}

Window::~Window() {
    CleanUp();
}

void Window::InitWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //Don't create a OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Window::FramebufferResizedCB);
}

bool Window::Update() {
    if (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    } else {
        running = false;
    }
    return running;
}

void Window::FramebufferResizedCB(GLFWwindow *window, int width, int height) {
    Window *currentWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    currentWindow->renderer->RecreateSwapchain();
}

void Window::CleanUp() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

GLFWwindow *Window::GetGLFWwindow() {
    return window;
}

const int Window::GetWidth() const {
    return WINDOW_WIDTH;
}

const int Window::GetHeight() const {
    return WINDOW_HEIGHT;
}
