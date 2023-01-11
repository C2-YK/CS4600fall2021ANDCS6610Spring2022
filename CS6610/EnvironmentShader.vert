#version 330 core

in vec4 vertexPosition;
uniform mat4 viewDirMatrix;

out vec3 frag_txc;

void main()
{
    frag_txc = (viewDirMatrix*vertexPosition).xyz;
    gl_Position = vertexPosition;
    gl_Position.z = 0.999999;
}