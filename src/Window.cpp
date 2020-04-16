#include "vkr/Window.h"


Window::Window(std::string name, int width, int height) 
                : name(name), 
                width(width), 
                height(height), 
                running(true), 
                OnResizedEvent([](){}) {
    Initialize();
}

Window::~Window() { 
    CleanUp(); 
}