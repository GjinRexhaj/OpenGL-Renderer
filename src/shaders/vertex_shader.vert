#version 460 core

// vertex shaders inherently need to have input data
// layout (location = 0) means that the input data is
// directly from the vertex data specified


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

//out vec3 ourColor;
out vec2 TexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   //gl_Position = transform * vec4(aPos, 1.0);
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}