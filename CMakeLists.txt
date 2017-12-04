cmake_minimum_required(VERSION 3.8)
project(vulkan_voxel)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Window.cpp Window.h Renderer.cpp Renderer.h BUILD_OPTIONS.h Utilities.cpp Utilities.h QueueFamilyIndices.cpp QueueFamilyIndices.h SwapChainSupportDetails.h)
add_executable(vulkan_voxel ${SOURCE_FILES})

#variables for libs
set(GLFW_DIR libs/glfw)


add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)
target_link_libraries(vulkan_voxel glfw ${GLFW_LIBRARIES})

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries(vulkan_voxel ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)