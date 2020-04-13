//
// Created by codewing on 22/02/2020.
//

#pragma once

#include <string>
#include <stb_image.h>

class Image {

private:
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* pixels = nullptr;

public:
    explicit Image(const char* path);
    virtual ~Image();

    size_t GetSize() const;
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

    void* GetData() const;
};
