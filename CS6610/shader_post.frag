#version 330 core
in vec4 frag_pos;
in vec3 frag_n;
in vec2 frag_txc;

uniform sampler2D textureMap;

void main(){
	gl_FragColor = vec4(texture2D(textureMap, frag_txc).rgb,1);
}