/*
 * shader.h
 *
 *  Created on: Jan 1, 2025
 *      Author: gjin
 *
 *      Header file for Shader class. Class which abstracts
 *      compiling GLSL code and linking it into a usable shader program.
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader {

public:
    // program ID
    unsigned int ID;

    // constructor, builds shader from paths
    Shader(const char* vertexPath, const char* fragmentPath);

    // destructor, calls glDeleteProgram
    ~Shader();

    // use/activate shader
    void use();

    // uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;


private:
    void checkCompileErrors(unsigned int shader, std::string type);


};

