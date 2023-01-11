#version 330 core
in vec3 m_pos;
in vec2 texc;
in vec3 m_normal;
in vec3 m_tangent;
in vec3 m_bitangent;

out vec2 texCoord;
out vec3 w_normal;
out vec3 v_normal;
out vec3 w_camera_dir;
out vec3 t_camera_dir;
out vec3 t_light;

uniform mat4 mvp;
uniform mat4 v;
uniform mat4 mv;
uniform mat4 m;

uniform sampler2D normal_tex;

void main()
{
    mat3 normal_m = transpose(inverse(mat3(m)));
    mat3 normal_mv = transpose(inverse(mat3(mv)));

    // vertex Normal
	w_normal = normalize(normal_m * m_normal);
    v_normal = normalize(normal_mv * m_normal);
    w_camera_dir = mat3(inverse(v)) * vec3(0,0,-1);

    // face Normal
    vec3 w_T = normalize(normal_mv * m_tangent);
    vec3 w_B = normalize(normal_mv * m_bitangent);
    vec3 w_N = normalize(cross(w_T, w_B));
    mat3 v_to_tan = inverse(mat3(-w_T, w_B, w_N));
    t_camera_dir = v_to_tan * vec3(0,0,-1);

    // t light
    vec3 light = vec3(-1, -1, -1);
    t_light = v_to_tan * light;

	gl_Position = mv * vec4(m_pos, 1.0);
	texCoord = texc;
}