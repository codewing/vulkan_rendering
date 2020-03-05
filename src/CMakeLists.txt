cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SOURCE_FILES "*.*")

add_executable(vulkan_voxel)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
target_sources(vulkan_voxel PRIVATE ${SOURCE_FILES})
target_compile_features(vulkan_voxel PUBLIC cxx_std_17)

# Libraries
target_link_libraries(vulkan_voxel PRIVATE Vulkan::Vulkan)

target_link_libraries(vulkan_voxel PRIVATE glfw)

target_link_libraries(vulkan_voxel PRIVATE glm)

target_link_libraries(vulkan_voxel PRIVATE stb_image)

# Dependency - Assets
add_dependencies(vulkan_voxel Shaders)
add_dependencies(vulkan_voxel Assets)