#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoord_GS[];
in vec3 w_normal_GS[];
in vec3 v_normal_GS[];
in vec3 w_camera_dir_GS[];
in vec3 t_camera_dir_GS[];
in vec3 t_light_GS[];

out vec2 texCoord_FS;
out vec3 w_normal;
out vec3 v_normal;
out vec3 w_camera_dir;
out vec3 t_camera_dir;
out vec3 t_light;

void main() {
    gl_Position = gl_in[0].gl_Position;
    texCoord_FS = texCoord_GS[0];
    w_normal = w_normal_GS[0];
    v_normal = v_normal_GS[0];
    w_camera_dir = w_camera_dir_GS[0];
    t_camera_dir = t_camera_dir_GS[0];
    t_light = t_light_GS[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    texCoord_FS = texCoord_GS[1];
    w_normal = w_normal_GS[1];
    v_normal = v_normal_GS[1];
    w_camera_dir = w_camera_dir_GS[1];
    t_camera_dir = t_camera_dir_GS[1];
    t_light = t_light_GS[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    texCoord_FS = texCoord_GS[2];
    w_normal = w_normal_GS[2];
    v_normal = v_normal_GS[2];
    w_camera_dir = w_camera_dir_GS[2];
    t_camera_dir = t_camera_dir_GS[2];
    t_light = t_light_GS[2];
    EmitVertex();

    EndPrimitive();
}