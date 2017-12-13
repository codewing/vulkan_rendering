//
// Created by codewing on 20/10/2017.
//

#ifndef VULKAN_VOXEL_WINDOW_H
#define VULKAN_VOXEL_WINDOW_H

class GLFWwindow;

class Window {

private:
    GLFWwindow* window = nullptr;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;

    bool running;

    void InitWindow();
    void CleanUp();

public:
    Window(int width, int height);
    ~Window();

    bool Update();
    GLFWwindow* GetGLFWwindow();

    const int GetWidth() const;

    const int GetHeight() const;
};


#endif //VULKAN_VOXEL_WINDOW_H
