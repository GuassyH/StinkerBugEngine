#ifndef DELTA_TIME_H
#define DELTA_TIME_H

#include "GLFW/glfw3.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

class DeltaTime {
private:
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    // Private constructor so only one instance can exist
    DeltaTime() {
        lastFrameTime = (float)glfwGetTime();
    }

public:
    static DeltaTime& getInstance() {
        static DeltaTime instance;
        return instance;
    }

    // Update deltaTime once per frame
    void update() {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
    }

    float get() const {
        return deltaTime;
    }

    // Delete copy constructors to prevent duplicates
    DeltaTime(const DeltaTime&) = delete;
    DeltaTime& operator=(const DeltaTime&) = delete;
};


#endif