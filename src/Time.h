//
// Created by codewing on 04.05.2018.
//

#pragma once

#include <chrono>

class Time {

private:
    float deltaSinceStart;
    float deltaTime;

public:
    Time();

    void UpdateTime();
    float GetDeltaTime();
    float GetTimeSinceStart();

};
