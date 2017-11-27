#include "SkyBox.h"


// Load texture with appropriate faces
// Specify the texture enum number (GL_TEXTUREx) to bind it
SkyBox::SkyBox(GLenum gl_texture_num, const GLchar* front_face, const GLchar* back_face, const GLchar* right_face, const GLchar* left_face, const GLchar* top_face, const GLchar* bottom_face)
	: cube_file_name("res/models/cube.obj")
{
	faces.push_back(right_face);
	faces.push_back(left_face);
	faces.push_back(top_face);
	faces.push_back(bottom_face);
	faces.push_back(back_face);
	faces.push_back(front_face);
	
	setUpTexture(gl_texture_num);
}


SkyBox::~SkyBox()
{
}

// Draw the skybox by binding the VAO
void SkyBox::drawSkyBox()
{
	glDepthMask(GL_FALSE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, cube_vertices.size());
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

// Loads the cube's vertices, sets up VAO and VBO, loads the cube map, and binds the texture
void SkyBox::setUpTexture(GLenum gl_texture_num)
{
	std::vector<glm::vec3> unused_normals;
	std::vector<glm::vec2> unused_UVs;
	loadOBJ(cube_file_name.c_str(), cube_vertices, unused_normals, unused_UVs);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(glm::vec3), &cube_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glActiveTexture(gl_texture_num);
	textureCubeMap = loadCubeMap(faces);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);

}

// Given a set of file names for the faces, it sets the appropriate texture parameters and returns the texture ID
GLuint SkyBox::loadCubeMap(std::vector<const GLchar*>& faces)
{
	GLuint texture;
	glGenTextures(1, &texture);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		SOIL_free_image_data(image); //free resources
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture;
}