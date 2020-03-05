##################################
#####   Adding Libraries     #####
##################################

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    set_target_properties(Vulkan::Vulkan PROPERTIES IMPORTED_GLOBAL true)
endif (Vulkan_FOUND)

add_subdirectory(glfw)

add_subdirectory(glm)

add_subdirectory(stb)

