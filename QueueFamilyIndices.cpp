//
// Created by codewing on 21/10/2017.
//

#include "QueueFamilyIndices.h"

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily >= 0;
}

int QueueFamilyIndices::getGraphicsFamily() {
    return graphicsFamily;
}

void QueueFamilyIndices::setGraphicsFamily(int graphicsFamily) {
    this->graphicsFamily = graphicsFamily;
}
