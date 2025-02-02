/*
 * TextureLoader.cpp
 *
 *  Created on: Jan 4, 2025
 *      Author: gjin
 *
 *      Implementation file for TextureLoader class, uses
 *      stb_image to load images as textures into OpenGL
 */

#include "TextureLoader.h"
#include <glad/glad.h>
#include "Logger.h"
#include <string>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



// TODO: BASIC IMPLEMENTATION, SEPERATE LOADING+BINDING TEXTURES LATER

// load and create a texture
TextureLoader::TextureLoader(std::string texturePath) {

    // generate and bind textures
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, generate mipmaps
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);


    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        Global::logger.log(INFO, "Texture loaded successfully.");
    } else {
        Global::logger.log(ERROR, "Texture failed to load!");
    }

    stbi_image_free(data);
}

// get texture ID of object
unsigned int TextureLoader::getTextureID() {
    return texture;
}

