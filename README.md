# Voxel Engine in Vulkan (wip)

## Setup

- Clone the repository
- Execute `git submodule init` and `git submodule update` to get the dependencies

## Compilation

- Add the `VULKAN_SDK` variable to your environment so CMake can find Vulkan
- Generate the build configuration with `cmake .`
- Build generated config folder with `make`