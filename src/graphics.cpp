/*
 * graphics.cpp
 *
 *  Created on: Dec 28, 2024
 *      Author: gjin
 *
 *      Implementation file for all OpenGL graphics rendering code.
 *      All attributes that are declared at a higher scope appear
 *      in more than one function.
 */


#include "Camera.h"
#include "Logger.h"
#include "Shader.h"
#include "graphics.h"


#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureLoader.h"


namespace graphics {


// declare higher scope items
GLuint VBO;
GLuint VAO;
GLuint EBO;

// shaders and textures
Shader* cube_shader;
TextureLoader* testTexture1;
TextureLoader* testTexture2;


void Prerender() {

    Global::logger.log(INFO, "Pre-rendering...");

    cube_shader = new Shader(
            "src/shaders/vertex_shader.vert", "src/shaders/fragment_shader.frag");


    // TEMP - FOR CUBE DEMONSTRATION
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // INITIALIZE VBO: DECLARED AT HIGHER SCOPE
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // INITIALIZE VAO: DECLARED AT HIGHER SCOPE
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // UNCOMMENT FOR WIREFRAME MODE
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load textures, append together via fragment shader - make higher scope
    testTexture1 = new TextureLoader("resources/textures/test-texture.png");
    testTexture2 = new TextureLoader("resources/textures/test-texture-underline.png");

    // activate shader
    cube_shader->use();

    // set uniforms for mix factor
    cube_shader->setInt("texture1", 0);
    cube_shader->setInt("texture2", 1);



    Global::logger.log(INFO, "Rendering...");
}


void Render() {

    float timeValue = glfwGetTime();

    // bind textures to appropriate texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, testTexture1->getTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, testTexture2->getTextureID());

    // activate shader
    cube_shader->use();

    // pass projection matrix to shader
    glm::mat4 projection = glm::perspective(glm::radians(GlobalCamera::camera.Zoom), (float)1280/(float)720, 0.1f, 100.0f);
    cube_shader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = GlobalCamera::camera.GetViewMatrix();
    cube_shader->setMat4("view", view);

    // calculate model for each object
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(timeValue*50), glm::vec3(0.0f, 1.0f, 0.0f));
    cube_shader->setMat4("model", model);

    // bind vertex array
    glBindVertexArray(VAO);

    // draw with indicies
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // draw w/o indicies
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // end bind vertex array
    glBindVertexArray(0);
}

void Cleanup() {
    Global::logger.log(INFO, "Cleanup, deleting vertex arrays.");
    glDeleteVertexArrays(1, &VAO);
    Global::logger.log(INFO, "Cleanup, deleting buffers.");
    glDeleteBuffers(1, &VBO);
    Global::logger.log(INFO, "Cleanup, deleting shader program.");

    delete cube_shader;
    delete testTexture1;
    delete testTexture2;

}

}
