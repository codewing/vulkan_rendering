//
// Created by codewing on 04.05.2018.
//

#ifndef VULKAN_VOXEL_TIME_H
#define VULKAN_VOXEL_TIME_H


#include <chrono>

class Time {

private:

    std::chrono::time_point startTime;
    std::chrono::time_point timeLastFrame;

    // Per frame updated values
    float deltaSinceStart;
    float deltaTime;

public:
    Time();

    void UpdateTime();
    float GetDeltaTime();
    float GetTimeSinceStart();

};


#endif //VULKAN_VOXEL_TIME_H
