#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>

class Shader
{
private:
	GLuint program_ID;

public:

	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	//Uses the current Shader
	void useProgram();

	// Getter for program ID
	GLuint getProgramID() const;

	// Utility uniform functions
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;
	// put other uniform variable stuff here

	// Utility attribute functions
	GLuint getLocAttrib(const std::string& name) const;
};

#endif // !SHADER_H


