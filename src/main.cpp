/*
 * main.cpp
 *
 *  Created on: Dec 26, 2024
 *      Author: gjin
 *
 *      Launching point of the application,
 *      contains all code needed to create a
 *      window using OpenGL and GLFW. Calls
 *      code from "framework" class at Imgui
 *      initialization and runtime loop.
 */


#include <iostream>
#include <cstdlib>
#include <string>

#include "Camera.h"
#include "Logger.h"
#include "graphics.h"
#include "framework.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// TODOs - In order of highest priority to lowest priority
// TODO: connect callback slot with resizing the frame buffer
// TODO: fix image stretching when scaling glfw and imgui window
// TODO: attach modal to "about" window
// TODO: modify logger to allow for concatenating cpp and c style strings
// TODO: make CMake file less verbose


// DEFINE CALLBACKS
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// camera variables
float lastX = 1280 / 2.0f;
float lastY = 720 / 2.0f;
bool firstMouse = true;



int main() {

    //////////////////
    // SETUP WINDOW //
    //////////////////
    Global::logger.log(INFO, "Program started.");


    if (!glfwInit()) {
        return 1;
    }

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only


    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "GLFW 3.4 Window", NULL, NULL);
    if (window == NULL) {
        return 1;
    }

    // Set context and bind callbacks
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwSwapInterval(1); // Enable vsync (fps cap too)

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw("Unable to context to OpenGL, failed to initialize GLAD");
    }

    int screen_width, screen_height;
    glfwGetFramebufferSize(window, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    char *GL_version= (char *)glGetString(GL_VERSION);
    char *GL_renderer=(char *)glGetString(GL_RENDERER);


    Global::logger.log(INFO, "Logging GL version and renderer:");
    Global::logger.log(INFO, GL_version);
    Global::logger.log(INFO, GL_renderer);

    /////////////////////////////////////////
    // INITIALIZE PROGRAM AND RUNTIME LOOP //
    /////////////////////////////////////////

    framework program;

    glEnable(GL_DEPTH_TEST);

    FrameBuffer *sceneBuffer;
    sceneBuffer = new FrameBuffer(screen_width, screen_height);
    program.Init(window, glsl_version);

    graphics::Prerender();

    while(!glfwWindowShouldClose(window)) {

        glfwPollEvents();


        program.NewFrame();
        program.Update(sceneBuffer);
        program.Render();
        sceneBuffer->Bind();

        // handle per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process keyboard input
        if(const auto& io = ImGui::GetIO(); !io.WantCaptureKeyboard) {
            processInput(window);
        }

        ///////////////////////
        // begin opengl code //
        ///////////////////////

        //glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(0.05f, 0.15f, 0.20f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        graphics::Render();
        /////////////////////
        // end opengl code //
        /////////////////////

        sceneBuffer->Unbind();
        glfwSwapBuffers(window);
    }

    Global::logger.log(INFO, "Program beginning exit sequence.");


    graphics::Cleanup();
    program.Shutdown();
    delete sceneBuffer;

    Global::logger.log(INFO, "Program terminated by user.\n\n\n");
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

    if(const auto& io = ImGui::GetIO(); !io.WantCaptureMouse) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        GlobalCamera::camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if(const auto& io = ImGui::GetIO(); !io.WantCaptureMouse) {
        GlobalCamera::camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        deltaTime *= 5;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        deltaTime /= 5;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        GlobalCamera::camera.ProcessKeyboard(UP, deltaTime);
}



/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Global::logger.log(DEBUG, "window_size_callback_static activated.");

    // these parameters represent the GLFW window size
    glViewport(0, 0, width, height);
    //sceneBuffer.RescaleFrameBuffer(width, height);


    // potentially create a global framebuffer so it's exposed to this callback function
    // may need to create this callback in framework.cpp as well
    // so changes to the imgui window size will also change this, could also
    // specify that changing imgui window size changes framebuffer size, resulting in callback
    //sceneBuffer.RescaleFrameBuffer(width, height);
}
*/

