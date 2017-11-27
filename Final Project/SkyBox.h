#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "Shader.h"
#include <vector>
#include "objloader.hpp"
#include "SOIL.h"

class SkyBox
{
private:
	const std::string cube_file_name;

	std::vector<glm::vec3> cube_vertices;
	std::vector<const GLchar*> faces; // For cubemap

	GLuint VAO;
	GLuint VBO_vertices;
	GLuint textureCubeMap;

	GLuint loadCubeMap(std::vector<const GLchar*>& faces);
	void setUpTexture(GLenum gl_texture_num);

public:
	SkyBox(GLenum gl_texture_num, const GLchar* front_face, const GLchar* back_face, const GLchar* right_face, const GLchar* left_face, const GLchar* top_face, const GLchar* bottom_face);
	~SkyBox();
	void drawSkyBox();
	//void activeAndBindTexture();
};

#endif

