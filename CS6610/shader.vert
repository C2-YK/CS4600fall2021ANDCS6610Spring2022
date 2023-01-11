#version 330 core

layout(location=0) in vec3 vert_pos;
layout(location=1) in vec3 vert_n;
layout(location=2) in vec2 vert_txc;

out vec4 frag_pos;
out vec3 frag_n;
out vec2 frag_txc;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 normal;

void main(){
	gl_Position = mvp * vec4(vert_pos, 1.0);
	frag_pos = mv * vec4(vert_pos, 1.0);
	frag_n = normal*vert_n;
	frag_txc = vert_txc;
}