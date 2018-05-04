//
// Created by codewing on 04.05.2018.
//

#include "Time.h"

Time::Time() {
    startTime = std::chrono::high_resolution_clock::now();
    timeLastFrame = startTime;
}

void Time::UpdateTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();

    deltaSinceStart = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - timeLastFrame).count();

    timeLastFrame = currentTime;
}

float Time::GetDeltaTime() {
    return deltaTime;
}

float Time::GetTimeSinceStart() {
    return deltaSinceStart;
}
