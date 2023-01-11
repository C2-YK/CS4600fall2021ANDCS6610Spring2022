#version 410 core

layout(triangles, equal_spacing, ccw) in;

in vec4 position_ES[];
in vec2 texCoord_ES[];
in vec3 w_normal_ES[];
in vec3 v_normal_ES[];
in vec3 w_camera_dir_ES[];
in vec3 t_camera_dir_ES[];
in vec3 t_light_ES[];

out vec2 texCoord_FS;
out vec3 v_normal;
out vec3 w_normal;
out vec3 w_camera_dir;
out vec3 t_camera_dir;
out vec3 t_light;

uniform mat4 p;
uniform sampler2D disp_tex;

void main()
{
	gl_Position = vec4(gl_TessCoord.x * gl_in[0].gl_Position +
	gl_TessCoord.y * gl_in[1].gl_Position +
	gl_TessCoord.z * gl_in[2].gl_Position);

	texCoord_FS = vec2(gl_TessCoord.x * texCoord_ES[0] +
	gl_TessCoord.y * texCoord_ES[1] +
	gl_TessCoord.z * texCoord_ES[2]);

	w_normal = vec3(gl_TessCoord.x * w_normal_ES[0] +
	gl_TessCoord.y * w_normal_ES[1] +
	gl_TessCoord.z * w_normal_ES[2]);

	v_normal = vec3(gl_TessCoord.x * v_normal_ES[0] +
	gl_TessCoord.y * v_normal_ES[1] +
	gl_TessCoord.z * v_normal_ES[2]);

	w_camera_dir = vec3(gl_TessCoord.x * w_camera_dir_ES[0] +
	gl_TessCoord.y * w_camera_dir_ES[1] +
	gl_TessCoord.z * w_camera_dir_ES[2]);

	t_camera_dir = vec3(gl_TessCoord.x * t_camera_dir_ES[0] +
	gl_TessCoord.y * t_camera_dir_ES[1] +
	gl_TessCoord.z * t_camera_dir_ES[2]);

	t_light = vec3(gl_TessCoord.x * t_light_ES[0] +
	gl_TessCoord.y * t_light_ES[1] +
	gl_TessCoord.z * t_light_ES[2]);

	float Displacement = texture(disp_tex, texCoord_FS.xy).x;

	gl_Position += vec4(v_normal_ES[0] * -Displacement * 10, 0);
	gl_Position.w = 1;
	gl_Position = p * gl_Position;
}