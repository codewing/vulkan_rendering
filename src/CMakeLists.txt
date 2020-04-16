cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_library(vkr)
target_sources(vkr PRIVATE ${SOURCE_FILES})

add_library(codewing::vkr ALIAS vkr)

target_include_directories(vkr
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
        )
target_compile_features(vkr PUBLIC cxx_std_17)

# Libraries
target_link_libraries(vkr PUBLIC Vulkan::Vulkan)
target_link_libraries(vkr PUBLIC glfw)
target_link_libraries(vkr PUBLIC glm)
target_link_libraries(vkr PUBLIC stb_image)
target_link_libraries(vkr PUBLIC tiny_obj_loader)

# Dependency - Assets
add_dependencies(vkr Shaders)
add_dependencies(vkr Assets)