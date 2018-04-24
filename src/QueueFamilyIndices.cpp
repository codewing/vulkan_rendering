//
// Created by codewing on 21/10/2017.
//

#include "QueueFamilyIndices.h"

bool QueueFamilyIndices::isGraphicsWithPresentFamilySet() {
    return graphicsFamily >= 0 && presentFamily >= 0;
}

int QueueFamilyIndices::GetGraphicsFamily() const {
    return graphicsFamily;
}

void QueueFamilyIndices::SetGraphicsFamily(int graphicsFamily) {
    this->graphicsFamily = graphicsFamily;
}

int QueueFamilyIndices::GetPresentFamily() const {
    return presentFamily;
}

void QueueFamilyIndices::SetPresentFamily(int presentFamily) {
    this->presentFamily = presentFamily;
}

int QueueFamilyIndices::GetTransferFamily() const {
    return transferFamily;
}

void QueueFamilyIndices::SetTransferFamily(int transferFamily) {
    this->transferFamily = transferFamily;
}
