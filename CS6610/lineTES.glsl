#version 410 core

layout(triangles, equal_spacing, ccw) in;

in vec3 pos_TES[];
in vec2 texCoord_ES[];
in vec3 w_normal_ES[];
in vec3 v_normal_ES[];

out vec3 pos_GS;

uniform sampler2D disp_tex;

uniform mat4 p;

vec4 interpolate(vec4 v0, vec4 v1, vec4 v2)
{
	vec4 a = mix(v1, v0, gl_TessCoord.x);
	vec4 b = mix(v2, v0, gl_TessCoord.y);
	return mix(a, b, gl_TessCoord.z);
}

void main()
{
	gl_Position = vec4(gl_TessCoord.x * gl_in[0].gl_Position +
	gl_TessCoord.y * gl_in[1].gl_Position +
	gl_TessCoord.z * gl_in[2].gl_Position);

	vec2 texCoord_GS = vec2(gl_TessCoord.x * texCoord_ES[0] +
	gl_TessCoord.y * texCoord_ES[1] +
	gl_TessCoord.z * texCoord_ES[2]);

	vec3 v_normal_u = vec3(gl_TessCoord.x * v_normal_ES[0] +
	gl_TessCoord.y * v_normal_ES[1] +
	gl_TessCoord.z * v_normal_ES[2]);

	float Displacement = texture(disp_tex, texCoord_GS.xy).x;

	gl_Position += vec4(v_normal_ES[0] * -Displacement * 10, 1);
	gl_Position = p * gl_Position;
}