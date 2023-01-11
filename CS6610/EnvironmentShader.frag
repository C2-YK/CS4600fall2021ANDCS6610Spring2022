#version 330 core

uniform samplerCube environmentMap;
in vec3 frag_txc;
out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(environmentMap, frag_txc);
}