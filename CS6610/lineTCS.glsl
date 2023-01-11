#version 410 core

layout(vertices = 3) out;

uniform int tes_level;

in vec2 texCoord[];
in vec3 w_normal[];
in vec3 v_normal[];

out vec2 texCoord_ES[];
out vec3 w_normal_ES[];
out vec3 v_normal_ES[];

void main(void)
{
	gl_TessLevelOuter[0] = tes_level;
	gl_TessLevelOuter[1] = tes_level;
	gl_TessLevelOuter[2] = tes_level;
	//gl_TessLevelOuter[3] = tes_level;

	gl_TessLevelInner[0] = tes_level;
	//gl_TessLevelOuter[1] = 8.0;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	texCoord_ES[gl_InvocationID] = texCoord[gl_InvocationID];
	w_normal_ES[gl_InvocationID] = w_normal[gl_InvocationID];
	v_normal_ES[gl_InvocationID] = v_normal[gl_InvocationID];
}