cmake_minimum_required(VERSION 3.8)
project(vulkan_voxel)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC
        "src/*.h"
        "src/*.cpp"
        )
set(SOURCE_FILES ${SRC})
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

# copying the compiled shaders to the output dir
file(GLOB SHADERS "shaders/*.spv")
file(COPY ${SHADERS} DESTINATION shaders)
