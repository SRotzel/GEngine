#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 texCoords;

out vec2 v_texCoords;

uniform mat4 u_mvp; // ModelViewProjection
uniform mat4 u_viewProj; 

void main()
{
    gl_Position = u_viewProj * u_mvp * pos;
    v_texCoords = texCoords;
}