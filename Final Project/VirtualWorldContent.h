#ifndef VIRTUALWORLDCONTENT_H
#define VIRTUALWORLDCONTENT_H

#include "DirectionalLight.h"
#include "Shader.h"
#include <vector>

class Object : public BaseObject
{
protected:
	glm::mat4 model_matrix;
	Color color;
	//static const GLfloat SCALE;
	float initial_scale_factor;

	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_normals;
	GLuint EBO; // Find out what to do with this

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	bool loadOBJ(
		const char* path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec3>& out_normals,
		std::vector<glm::vec2>& out_uvs
		);

	void setUpObject();

	void setModelMatrix();
	void setColor();

					 // The states of update
	virtual void initState();
	virtual void modState() = 0; // This is the real meat of the updating
	virtual void drawState();

	void setCollisionRadius();

public:

	Object(const std::string& file_name, Shader* shader);
	virtual ~Object();

	virtual void update();

	void scale(const glm::vec3& scale);
	void scale(GLfloat scale);
	void translate(const glm::vec3& translate_vect);
	void rotate(const glm::vec3& rotation_axis, GLfloat angle_deg);
	void destroy();
	bool isDestroyed();

	glm::vec3 getPosition() const;
};

class PolarBear : public Object
{
private:
	glm::vec3 move;
	const float SCALE_POLAR_BEAR = 0.032f;
public:
	PolarBear(Shader* shader, const glm::vec3& m);
	~PolarBear();
	void modState();
};

class Eskimo : public Object
{
private:
	const float SCALE_ESKIMO = 0.2f;
	glm::vec3 move;
public:
	Eskimo(Shader* shader, const glm::vec3& m);
	~Eskimo();
	void modState();
};

class Penguin : public Object
{
private:
	const float SCALE_PENGUIN = 1.0f;
	glm::vec3 move;
public:
	Penguin(Shader* shader, const glm::vec3& m);
	~Penguin();
	void modState();
};

class Igloo : public Object
{
private:
	const float SCALE_IGLOO = 1.0f;
	glm::vec3 move;
public:
	Igloo(Shader* shader, const glm::vec3& m);
	~Igloo();
	void modState();
};

#endif
