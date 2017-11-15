#ifndef OBJECT_H
#define OBJECT_H

#include "Shader.h"
#include <vector>

class Object
{
protected:
	glm::mat4 model_matrix;
	glm::vec3 color; // To be modified when we implement light

	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_normals;
	GLuint EBO; // Find out what to do with this

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	Shader* shader;

	bool loadOBJ(
		const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec3>& out_normals,
		std::vector<glm::vec2>& out_uvs
	);

	void setUpObject();
	
	void setModelMatrix();
	void setColor(); // To be modified when we implement light

	// The states of update
	virtual void initState();
	virtual void modState() = 0; // This is the real meat of the updating
	virtual void drawState();

public:

	static const GLfloat SCALE;

	Object(const std::string& file_name, Shader* shader);
	virtual ~Object();

	virtual void update();

	void scale(const glm::vec3& scale);
	void scale(GLfloat scale);
	void translate(const glm::vec3& translate_vect);
	void rotate(const glm::vec3& rotation_axis, GLfloat angle_deg);
	void destroy();
	// bool testCollision(const Object& other); 
};

const GLfloat Object::SCALE = 0.05;

#endif
