cmake_minimum_required(VERSION 3.12)

add_executable(VulkanSandbox sandbox.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
target_compile_features(VulkanSandbox PRIVATE cxx_std_17)
target_link_libraries(VulkanSandbox PRIVATE codewing::vkr)