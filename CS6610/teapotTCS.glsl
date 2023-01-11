#version 410 core

layout(vertices = 3) out;

uniform int tes_level;

in vec2 texCoord[];
in vec3 w_normal[];
in vec3 v_normal[];
in vec3 w_camera_dir[];
in vec3 t_camera_dir[];
in vec3 t_light[];

out vec2 texCoord_ES[];
out vec3 w_normal_ES[];
out vec3 v_normal_ES[];
out vec3 w_camera_dir_ES[];
out vec3 t_camera_dir_ES[];
out vec3 t_light_ES[];

void main(void)
{
	gl_TessLevelOuter[0] = tes_level;
	gl_TessLevelOuter[1] = tes_level;
	gl_TessLevelOuter[2] = tes_level;

	gl_TessLevelInner[0] = tes_level;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	texCoord_ES[gl_InvocationID] = texCoord[gl_InvocationID];
	w_normal_ES[gl_InvocationID] = w_normal[gl_InvocationID];
	v_normal_ES[gl_InvocationID] = v_normal[gl_InvocationID];
	w_camera_dir_ES[gl_InvocationID] = w_camera_dir[gl_InvocationID];
	t_camera_dir_ES[gl_InvocationID] = t_camera_dir[gl_InvocationID];
	t_light_ES[gl_InvocationID] = t_light[gl_InvocationID];
}