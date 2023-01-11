#version 330 core
in vec3 frag_pos;
in vec3 frag_n;
in vec2 frag_txc;
//Light Attributes
uniform vec3 camera_position;
uniform vec3 ambient;
uniform vec3 light_color;
uniform vec3 light_pos;
//Reflective Attributes
uniform samplerCube environmentMap;
//Material Attributes
uniform float hasTexture;
uniform float specular_alpha;
uniform vec3 specular_color;
uniform vec3 diffuse_color;
uniform sampler2D textureMap;

void main(){
	vec3 frag_view = normalize(camera_position - frag_pos);
	vec3 light_dir = normalize(light_pos - frag_pos);
	//obj Texture
	vec3 normal = normalize(frag_n);
    vec3 diffuse = diffuse_color;
    if(hasTexture>0){
        diffuse = texture2D(textureMap, frag_txc).rgb;
    }
	vec3 light = light_color * max(dot(light_dir, normal), 0.0);
	vec3 h = normalize(light_dir + frag_view);
	vec3 specular = pow(max(dot(normal, h), 0.0), specular_alpha)*specular_color;
	vec3 rgb = ambient * diffuse + light * (diffuse + specular);
	//obj Reflective
	vec3 ref_txc = reflect(-frag_view, normal);
    vec3 ref_diffuse = texture(environmentMap, ref_txc).xyz;
	rgb += ref_diffuse * specular_color;
	gl_FragColor = vec4(rgb, 1);
}