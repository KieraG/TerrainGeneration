#pragma once

#include "glm/vec3.hpp"

using glm::vec3;

class Camera {
  public:
    vec3 position = {0, 1, 0};
    vec3 look     = {0, -1, 0};
    vec3 upVec    = {0, 0, 0};
    float pitch   = 90.f;
    float yaw     = 90.f;
    void moveForward();
    void moveBack();
    void moveLeft();
    void moveRight();
    void updateCameraLook();
    static Camera& getInstance() {
        static auto instance = Camera{};

        return instance;
    }
};