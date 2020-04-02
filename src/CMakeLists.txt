cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SOURCE_FILES "*.*")

add_executable(vulkan_rendering)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
target_sources(vulkan_rendering PRIVATE ${SOURCE_FILES})
target_compile_features(vulkan_rendering PUBLIC cxx_std_17)

# Libraries
target_link_libraries(vulkan_rendering PRIVATE Vulkan::Vulkan)

target_link_libraries(vulkan_rendering PRIVATE glfw)

target_link_libraries(vulkan_rendering PRIVATE glm)

target_link_libraries(vulkan_rendering PRIVATE stb_image)

target_link_libraries(vulkan_rendering PRIVATE tiny_obj_loader)

# Dependency - Assets
add_dependencies(vulkan_rendering Shaders)
add_dependencies(vulkan_rendering Assets)