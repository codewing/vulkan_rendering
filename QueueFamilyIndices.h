//
// Created by codewing on 21/10/2017.
//

#ifndef VULKAN_VOXEL_QUEUEFAMILYINDICES_H
#define VULKAN_VOXEL_QUEUEFAMILYINDICES_H


class QueueFamilyIndices {

public:
    const int FAMILY_NOT_FOUND = -1;

    bool isComplete();
    int getGraphicsFamily();
    void setGraphicsFamily(int graphicsFamily);

private:
    int graphicsFamily = FAMILY_NOT_FOUND;

};


#endif //VULKAN_VOXEL_QUEUEFAMILYINDICES_H
