//
// Created by codewing on 21/10/2017.
//

#ifndef VULKAN_VOXEL_QUEUEFAMILYINDICES_H
#define VULKAN_VOXEL_QUEUEFAMILYINDICES_H


class QueueFamilyIndices {

public:
    const int NOT_FOUND_OR_UNSET = -1;

    bool isGraphicsWithPresentFamilySet();

private:
    int graphicsFamily = NOT_FOUND_OR_UNSET;
    int presentFamily = NOT_FOUND_OR_UNSET;
    int transferFamily = NOT_FOUND_OR_UNSET;

public:
    int GetGraphicsFamily() const;
    void SetGraphicsFamily(int graphicsFamily);

    int GetPresentFamily() const;
    void SetPresentFamily(int presentFamily);

    int GetTransferFamily() const;
    void SetTransferFamily(int transferFamily);
};


#endif //VULKAN_VOXEL_QUEUEFAMILYINDICES_H
