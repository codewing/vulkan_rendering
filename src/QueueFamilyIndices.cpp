//
// Created by codewing on 21/10/2017.
//

#include "QueueFamilyIndices.h"

bool QueueFamilyIndices::isGraphicsWithPresentFamilySet() {
    return graphicsFamily != UINT32_MAX && presentFamily != UINT32_MAX;
}

uint32_t QueueFamilyIndices::GetGraphicsFamily() const {
    return graphicsFamily;
}

void QueueFamilyIndices::SetGraphicsFamily(uint32_t graphicsFamily) {
    this->graphicsFamily = graphicsFamily;
}

uint32_t QueueFamilyIndices::GetPresentFamily() const {
    return presentFamily;
}

void QueueFamilyIndices::SetPresentFamily(uint32_t presentFamily) {
    this->presentFamily = presentFamily;
}

uint32_t QueueFamilyIndices::GetTransferFamily() const {
    return transferFamily;
}

void QueueFamilyIndices::SetTransferFamily(uint32_t transferFamily) {
    this->transferFamily = transferFamily;
}
