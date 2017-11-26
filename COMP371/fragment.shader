#version 330 core

in vec3 colorOut;
out vec4 color;
uniform int object_type;

void main()
{
	//white
	if(object_type == 0){
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// red
	if(object_type == 1){
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	//yellow
	if(object_type == 2){
		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	//Origin colors
	if (object_type ==3){
		color = vec4(colorOut, 1.0f);
	}
} 