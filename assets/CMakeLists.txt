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
    set(SPIRV_FILE "${PROJECT_BINARY_DIR}/assets/shaders/${FILE_NAME}.spv")
    message("${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE}")
    add_custom_command(
            OUTPUT ${SPIRV_FILE}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/assets/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE}
            DEPENDS ${GLSL_FILE})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach (GLSL_FILE)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

##################################
#####   Copying the assets   #####
##################################

set(ASSET_FOLDERS textures models)

foreach(FOLDER ${ASSET_FOLDERS})

    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/assets/${FOLDER}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets/${FOLDER}
                ${PROJECT_BINARY_DIR}/assets/${FOLDER}
    )
    list(APPEND ASSET_BINARY_FOLDERS ${PROJECT_BINARY_DIR}/assets/${FOLDER})

endforeach()

add_custom_target(
        Assets
        DEPENDS ${ASSET_BINARY_FOLDERS}
)
