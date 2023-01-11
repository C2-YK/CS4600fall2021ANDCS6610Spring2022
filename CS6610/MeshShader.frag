#version 330 core
in vec3 frag_pos;
in vec3 frag_n;
in vec2 frag_txc;
in vec4 frag_lightPos;
//Light Attributes
uniform vec3 camera_position;
uniform vec3 ambient;
uniform vec3 light_color;
uniform vec3 light_pos;
//Material Attributes
uniform float hasTexture;
uniform float specular_alpha;
uniform vec3 specular_color;
uniform vec3 diffuse_color;
uniform sampler2D textureMap;
uniform sampler2D shadowMap;

float shadowCalc()
{
	vec3 pos = frag_lightPos.xyz * 0.5 + 0.5;
	float depth = texture(shadowMap, pos.xy).r;
	return depth < pos.z ? 0.0 : 1.0;
}

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

	//calculate shadow
	float shadow = shadowCalc();
	vec3 rgb = ambient * diffuse + light * (diffuse + specular) * shadow;
	float depthValue = texture(shadowMap, frag_txc).r;

	//gl_FragColor = vec4(rgb,1);
	gl_FragColor = vec4(vec3(depthValue),1);
}