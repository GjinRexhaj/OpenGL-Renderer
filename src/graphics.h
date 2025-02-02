/*
 * graphics.h
 *
 *  Created on: Dec 28, 2024
 *      Author: gjin
 *
 *      Header file for all OpenGL graphics rendering code.
 *      Prerender function handles the compilation and linkage of
 *      shaders, VBOs/VAOs, verticies, etc. Render function handles
 *      the runtime behavior of graphics, and cleanup is called
 *      when the program is terminated.
 */

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace graphics {

void Prerender();
void Render();
void Cleanup();

}
