
#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform int object_type;

out vec3 colorOut;

void main()
{
	if (object_type == 0 || object_type == 3 ){
		gl_Position = projection_matrix * view_matrix * vec4(position.x, position.y, position.z, 1.0);
	}	
	else{
		gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
	}
	colorOut = color;
}