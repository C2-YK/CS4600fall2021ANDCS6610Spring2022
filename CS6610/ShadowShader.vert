#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 lp;
uniform mat4 m;

void main() {
    gl_Position = lp * m * vec4(aPos, 1.0);
}