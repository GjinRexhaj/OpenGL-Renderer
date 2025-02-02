/*
 * framework.h
 *
 *  Created on: Dec 26, 2024
 *      Author: gjin
 *
 *      Header file for the "framework" class.
 *      This class contains all code needed to initalize
 *      and actively run all Imgui elements in a GLFW
 *      window using the OpenGL backend.
 */

#pragma once

#include "FrameBuffer.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Logger.h"


class framework {

public:
    void Init(GLFWwindow* window, const char* glsl_version);
    void NewFrame();
    void Update(FrameBuffer* sceneBuffer);
    void Render();
    void Shutdown();
};

