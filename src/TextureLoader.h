/*
 * TextureLoader.cpp
 *
 *  Created on: Jan 4, 2025
 *      Author: gjin
 *
 *      Header  file for TextureLoader class, uses
 *      stb_image to load images as textures into OpenGL
 */


#pragma once

#include <string>


class TextureLoader {

public:

    // constructor
    TextureLoader(std::string texturePath);
    unsigned int getTextureID();


private:

    unsigned int texture;


};
