#version 330 core

struct Color {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 frag_position;
uniform Color our_color;
uniform vec3 view_pos;
uniform Color light_color;
uniform vec3 light_position;
out vec4 color;

void main()
{
	vec3 ambient = light_color.ambient * our_color.ambient;
	
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_position - frag_position);
	
	float diff = max(dot(norm, light_dir), 0.0f);
	vec3 diffuse = light_color.diffuse * (diff * our_color.diffuse);
	
	float specular_strength = 0.5;
	vec3 view_dir = normalize(view_pos - frag_position);
	vec3 reflect_dir = reflect(-light_dir, norm);
	
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
	vec3 specular = light_color.specular * (spec * our_color.specular);
	
	vec3 final_color = ambient + diffuse + specular;
    color = vec4(final_color, 1.0f);
} 