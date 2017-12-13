//
// Created by codewing on 20/10/2017.
//
#pragma once

#ifndef VULKAN_VOXEL_UTILITIES_H
#define VULKAN_VOXEL_UTILITIES_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

void ErrorCheck(VkResult result);

#endif //VULKAN_VOXEL_UTILITIES_H

std::vector<char> readFile(const std::string& filename);
