#version 330 core
in vec4 frag_pos;
in vec3 frag_n;
in vec2 frag_txc;

void main(){
	vec3 light_dir = normalize(vec3(0.5, 1, -0.5));
	vec3 diffuse = vec3(0.8,0.8,0.8);
	vec3 ambient = diffuse * 0.2;
	vec3 light = vec3(0.9,0.9,0.9) * max(dot(light_dir, frag_n), 0.0);
	vec3 h = normalize(light_dir - frag_pos.xyz);
	vec3 specularColor = vec3(1,1,1);
	float alpha = 500;
	vec3 specular = (pow(max(dot(frag_n, h), 0.0), alpha)/dot(frag_n, light_dir))*specularColor;
	vec3 rgb = ambient + light * (diffuse + specular);
	gl_FragColor = vec4(rgb,1);
}