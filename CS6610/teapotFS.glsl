#version 330 core
out vec4 FragColor;

in vec2 texCoord_FS;
in vec3 v_normal;
in vec3 w_normal;
in vec3 t_camera_dir;
in vec3 t_light;

uniform mat4 v;
uniform mat4 mv;

uniform sampler2D normal_tex;
uniform sampler2D disp_tex;

void main()
{
    vec3 t_normal = texture(normal_tex, texCoord_FS).xyz;
    t_normal = normalize(t_normal * 2.0 - 1.0);
    
    vec4 ambient = vec4(.2, 0, 0, 1);
    vec3 diffuse = pow(max(dot(t_light, t_normal), 0), 2) * vec3(1, 0, 0);
    vec3 half = normalize((t_light + t_camera_dir)/length(t_light + t_camera_dir));
    vec4 blinn = vec4(vec3(1, 1, 1) * pow(dot(half, t_normal), 30), 0.);

    FragColor = clamp(ambient + vec4(diffuse, 0) + clamp(blinn, 0, 1), 0, 1);
    FragColor.a = 1.;
    //FragColor = texture(disp_tex, texCoord_FS);
    //FragColor = vec4(diffuse, 0);
    //FragColor = vec4(t_camera_dir, 0);
    //FragColor = vec4(t_normal, 0);
} 