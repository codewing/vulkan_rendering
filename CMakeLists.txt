cmake_minimum_required(VERSION 3.8)
project(vulkan_voxel)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/main.cpp
        src/BUILD_OPTIONS.h
        src/Renderer.cpp src/Renderer.h
        src/QueueFamilyIndices.cpp
        src/QueueFamilyIndices.h
        src/SwapChainSupportDetails.h
        src/Utilities.cpp src/Utilities.h
        src/Window.cpp src/Window.h src/vertex.h src/Scene.cpp src/Scene.h)
add_executable(vulkan_voxel ${SOURCE_FILES})

##################################
#####   Adding Libraries     #####
##################################

set(GLFW_DIR    libs/glfw)
set(GLM_DIR     libs/glm)

add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)
target_link_libraries(vulkan_voxel glfw ${GLFW_LIBRARIES})

include_directories(${GLM_DIR})

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries(vulkan_voxel ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)

##################################
##### Compiling the Shaders  #####
##################################

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
else ()
    message("Unsupported build platform")
endif ()


file(GLOB_RECURSE GLSL_SOURCE_FILES
        "shaders/*.frag"
        "shaders/*.vert"
        )

foreach (GLSL_FILE ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL_FILE} NAME)
    set(SPIRV_FILE "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    message("${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE}")
    add_custom_command(
            OUTPUT ${SPIRV_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE}
            DEPENDS ${GLSL_FILE})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach (GLSL_FILE)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders)

