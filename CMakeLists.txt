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

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/shaders)
file(GLOB SHADERS_SRC "shaders/shader.*")
foreach(SHADER_SRC ${SHADERS_SRC})
    string(REGEX MATCH "shader.([a-z]+)" IGNORED ${SHADER_SRC})
    set(SHADER_COMPILED "${CMAKE_BINARY_DIR}/shaders/${CMAKE_MATCH_1}.spv")
    execute_process(COMMAND glslangValidator -V ${SHADER_SRC} -o ${SHADER_COMPILED}
            INPUT_FILE ${SHADER_SRC}
            OUTPUT_FILE ${SHADER_COMPILED}
            WORKING_DIRECTORY ${VULKAN_SDK}/bin
            RESULT_VARIABLE res_var)
endforeach()
