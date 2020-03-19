#pragma once

#include "Camera.h"

#include <iostream>

#include <glm\trigonometric.hpp>

void Camera::moveForward() {}

void Camera::moveBack() {}

void Camera::moveLeft() {}

void Camera::moveRight() {}

void Camera::updateCameraLook() {
    if (pitch > 180) {
        pitch = 180;
    } else if (pitch < 0) {
        pitch = 0;
    }
    look.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)) + position.x;
    look.y = sin(glm::radians(pitch)) + position.y;
    look.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)) + position.z;
    std::cout << "Pitch: " << pitch << " Yaw: " << yaw << std::endl;
    std::cout << look.x << ' ' << look.y << ' ' << look.z << std::endl;
    std::cout << position.x << ' ' << position.y << ' ' << position.z << std::endl;
}