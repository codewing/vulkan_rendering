//
// Created by codewing on 06/11/2017.
//

#ifndef VULKAN_VOXEL_SWAPCHAINSUPPORTDETAILS_H
#define VULKAN_VOXEL_SWAPCHAINSUPPORTDETAILS_H


#include <vulkan/vulkan.h>
#include <vector>

class SwapChainSupportDetails {

private:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


#endif //VULKAN_VOXEL_SWAPCHAINSUPPORTDETAILS_H
