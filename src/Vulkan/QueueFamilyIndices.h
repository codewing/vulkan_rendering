//
// Created by codewing on 21/10/2017.
//

#pragma once

#include <cstdint>

class QueueFamilyIndices {

public:
    const uint32_t NOT_FOUND_OR_UNSET = UINT32_MAX;

    bool isGraphicsWithPresentFamilySet();

private:
    uint32_t graphicsFamily = NOT_FOUND_OR_UNSET;
    uint32_t presentFamily = NOT_FOUND_OR_UNSET;
    uint32_t transferFamily = NOT_FOUND_OR_UNSET;

public:
    uint32_t GetGraphicsFamily() const;

    void SetGraphicsFamily(uint32_t graphicsFamily);

    uint32_t GetPresentFamily() const;

    void SetPresentFamily(uint32_t presentFamily);

    uint32_t GetTransferFamily() const;

    void SetTransferFamily(uint32_t transferFamily);
};
