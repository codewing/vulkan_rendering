//
// Created by codewing on 22/02/2020.
//
#define STB_IMAGE_IMPLEMENTATION

#include "vkr/Image.h"

#include <stdexcept>

Image::Image(const char* filename) {
    pixels = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels) {
        throw std::runtime_error("failed to load texture image: " + std::string(filename));
    }
}

Image::~Image() {
    stbi_image_free(pixels);
}

size_t Image::GetSize() const {
    return static_cast<size_t>(width * height * 4);
}

void* Image::GetData() const {
    return pixels;
}
