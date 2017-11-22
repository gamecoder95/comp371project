#include "stdafx.h"

#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensures ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	//2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Shader Program
	this->program_ID = glCreateProgram();
	glAttachShader(this->program_ID, vertex);
	glAttachShader(this->program_ID, fragment);
	glLinkProgram(this->program_ID);
	// Print linking errors if any
	glGetProgramiv(this->program_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Delete the shaders as they're linked into our program and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::useProgram()
{
	glUseProgram(this->program_ID);
}

GLuint Shader::getProgramID() const
{
	return program_ID;
}

// Useful uniform setting functions
// and attribute location obtaining function

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(program_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(program_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::getLocAttrib(const std::string& name) const
{
	return glGetAttribLocation(program_ID, name.c_str());
}