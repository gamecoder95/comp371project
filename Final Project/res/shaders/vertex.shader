
#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 in_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec3 frag_position;
out vec3 normal;

void main()
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
	frag_position = vec3(model_matrix * vec4(position, 1.0f));
	normal = in_normal;
}