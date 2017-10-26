//
// Created by codewing on 21/10/2017.
//

#ifndef VULKAN_VOXEL_QUEUEFAMILYINDICES_H
#define VULKAN_VOXEL_QUEUEFAMILYINDICES_H


class QueueFamilyIndices {

public:
    const int NOT_FOUND_OR_UNSET = -1;

    bool isComplete();

    int GetGraphicsFamily() const;
    void SetGraphicsFamily(int graphicsFamily);

    int GetPresentFamily() const;
    void SetPresentFamily(int presentFamily);

private:
    int graphicsFamily = NOT_FOUND_OR_UNSET;
    int presentFamily = NOT_FOUND_OR_UNSET;

};


#endif //VULKAN_VOXEL_QUEUEFAMILYINDICES_H
