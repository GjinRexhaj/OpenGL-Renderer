/*
 * framework.cpp
 *
 *  Created on: Dec 26, 2024
 *      Author: gjin
 *
 *      Implementation file for the "framework" class.
 *      This class contains all code needed to initalize
 *      and actively run all Imgui elements in a GLFW
 *      window using the OpenGL backend.
 */

#include "framework.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <imgui.h>
#include "Logger.h"

// declare fonts at high scope
ImFont* font_regular;
ImFont* font_console;


void framework::Init(GLFWwindow* window, const char* glsl_version) {

    // basic setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // add dockspace and menu
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // setup platform/render bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // theming
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.f;
    style.TabRounding = 8.f;
    style.FrameRounding = 8.f;
    style.ChildRounding = 8.f;
    style.GrabRounding = 8.f;
    style.PopupRounding = 8.f;

    // font
    font_regular = io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Regular.ttf", 18);
    font_console = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", 18);

    // log
    Global::logger.log(INFO, "ImGui initialized.");

}

void framework::NewFrame() {

    // feed input to imgui. and start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

///////////////////////////////////////////////////
// PLACE ALL USER INTERFACE CODE IN BELOW METHOD //
///////////////////////////////////////////////////
void framework::Update(FrameBuffer* sceneBuffer) {

    // below is sample code
    static bool show_scene_window = true;
    static bool show_about_window = false;
    static bool show_demo_window = false;
    static bool show_debug_window = false;
    static bool show_console_window = true;
    static bool show_performance_window = true;
    static bool show_controls_window = false;
    static int scroll = Global::GLlogBuffer.size();
    static float fps;
    static float fpsms;
    static float GL_window_width;
    static float GL_window_height;


    // create fullscreen window with tabs
    if (ImGui::BeginMainMenuBar()) {
        /*
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save")) {}
            if (ImGui::MenuItem("Save As")) {}
            ImGui::EndMenu();
        }
        */
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("OpenGL Scene")) { if (!show_scene_window){show_scene_window = true;}}
            if (ImGui::MenuItem("Console Log")) { if (!show_console_window){show_console_window = true;}}
            if (ImGui::MenuItem("Performance")) { if (!show_performance_window){show_performance_window = true;}}

            ImGui::Separator();
            if (ImGui::MenuItem("Imgui Demo")) { if (!show_demo_window){show_demo_window = true;}}
            if (ImGui::MenuItem("Imgui Debug")) { if (!show_debug_window){show_debug_window = true;}}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Other")) {
            if (ImGui::MenuItem("About")) { if (!show_about_window){show_about_window = true;}}
            if (ImGui::MenuItem("Controls")) { if (!show_controls_window){show_controls_window = true;}}
            ImGui::EndMenu();

        }
        ImGui::EndMainMenuBar();
    }

    // setup dockspace
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());


    // show demo window
    if (show_demo_window) {
        ImGui::ShowDemoWindow();
    }

    // show imgui debug window
    if (show_debug_window) {
        ImGui::ShowDebugLogWindow();
    }

    // show console log window
    if (show_console_window) {
        if (!ImGui::Begin("Logging Console", &show_console_window)) {
            ImGui::End();
        } else {

            if (ImGui::Button("Clear console")) { Global::GLlogBuffer.clear();  }
            if (ImGui::BeginItemTooltip()) {
                ImGui::SetTooltip("This action will not remove contents from the log file.");
                ImGui::EndTooltip();
            }




            ImGui::SameLine(120.f);
            ImGui::Text("Log buffer contents: %d bytes", Global::GLlogBuffer.size());

            ImGui::BeginChild("log", ImVec2(0,0), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::PushFont(font_console);

            ImGui::TextUnformatted(Global::GLlogBuffer.begin(), Global::GLlogBuffer.end());

            ImGui::PopFont();

            if (Global::GLlogBuffer.size() != scroll) {
                ImGui::SetScrollHereY(1.0f);
                scroll = Global::GLlogBuffer.size();
            }



            ImGui::EndChild();
            ImGui::End();
        }
    }

    // show performance window
    if (show_performance_window) {
        if (!ImGui::Begin("Performance Metrics", &show_performance_window)) {
            ImGui::End();
        } else {
            // display fps and ms
            fps = ImGui::GetIO().Framerate;
            fpsms = 1000.0f / fps;


            ImGui::Text("GL RESL: %.0fx%.0f", GL_window_width, GL_window_height);
            ImGui::Text("UPTIME: %.0f seconds", ImGui::GetTime());

            // find way to display ms per frame

            // frame-rate graph
            // 60FPS max
            static float speed = 1.0f;
            static float average;


            ImGui::Text("Graph update speed -> %.1f", speed);
            ImGui::SliderFloat(" ", &speed, 1.f, 60.f);

            ImGui::BeginChild("FPS graph", ImVec2(0,0), ImGuiChildFlags_Border);

            ImGui::Text("Current FPS: %.2f", fps, 1.0f, 60.0f);
            ImGui::Text("Graph Average: %.2f", average);
            ImGui::Text("ms per frame: %.2f", fpsms, 1.0f, 60.0f);

            static float values [60];
            static int values_offset = 0;
            static double refresh_time = 0.0;

            while (refresh_time < ImGui::GetTime()) {

                values[values_offset] = fps;
                values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
                refresh_time += 1.0f/speed;
            }
            average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(values); n++) {
                average += values[n];
            }
            average /= (float)IM_ARRAYSIZE(values);

            char overlay[32];
            sprintf(overlay, " ");

            ImGui::PlotLines(" ", values, IM_ARRAYSIZE(values), values_offset, overlay, 0.0f, 60.0f, ImVec2(0, 80.0f));



            ImGui::EndChild();

            ImGui::End();
        }
    }


    // show about window
    if (show_about_window) {
        if (!ImGui::Begin("About", &show_about_window)) {
            ImGui::End();
        } else {

            ImGui::Text("Learning OpenGL");
            ImGui::Separator();
            ImGui::Text("Written by Gjin Rexhaj");
            ImGui::Text("Using ImGui 1.91.7 - docking branch");

            ImGui::End();
        }
    }

    // show controls window
    if (show_controls_window) {
        if (!ImGui::Begin("Controls", &show_controls_window)) {
            ImGui::End();
        } else {

            ImGui::Text("OpenGL camera controls");

            ImGui::BeginChild("opengl control", ImVec2(0,0), ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::Text("W: forward");
            ImGui::Text("A: left");
            ImGui::Text("S: right");
            ImGui::Text("D: backward");

            ImGui::Text("Q: Downwards");
            ImGui::Text("E: Upwards");
            ImGui::Text("LShift: Increased camera speed (while holding)");
            ImGui::Text("LCtrl: Decreased camera speed (while holding)");

            ImGui::EndChild();

            ImGui::End();
        }
    }

    // show framebuffer window
    if (show_scene_window) {
        if (!ImGui::Begin("OpenGL Scene View", &show_scene_window)) {
            ImGui::End();
        } else {
            ImGui::BeginChild("Render");


            // these variables represent window dimenstions
            GL_window_width = ImGui::GetContentRegionAvail().x;
            GL_window_height = ImGui::GetContentRegionAvail().y;


            ImGui::Image(

                        //(ImTextureID)sceneBuffer->getFrameTexture(),
                        static_cast<ImTextureID>(sceneBuffer->getFrameTexture()),

                        ImGui::GetContentRegionAvail(),
                        ImVec2(0,1),
                        ImVec2(1,0)
            );


            ImGui::EndChild();
            ImGui::End();
        }
    }
}
////////////////////////////////////
// CUSTOM USER-INTERFACE CODE END //
////////////////////////////////////



void framework::Render() {

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


void framework::Shutdown() {

    // cleanup

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
