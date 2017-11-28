#include "stdafx.h"

#include "VirtualWorldContent.h"
#include <stdio.h>
#include <cstring>
#include <iostream>


// static constants here
//const GLfloat Object::SCALE = 0.1f;

Object::Object(const std::string& file_name, Shader* shader)
	: BaseObject(shader)
{
	// Improve error-handling here
	if (loadOBJ(file_name.c_str(), vertices, normals, UVs))
	{
		setUpObject();
	}
	model_matrix = glm::mat4(1.0f);
}


Object::~Object()
{
	destroy();
}

void Object::setUpObject()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("position"));// Replace with non-hard-coded value

	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("in_normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("in_normal"));// Replace with non-hard-coded value

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Collision sphere
	setCollisionBox();
}

void Object::destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_vertices);
	glDeleteBuffers(1, &VBO_normals);
	VAO = VBO_vertices = VBO_normals = 0;
}

bool Object::isDestroyed()
{
	return VAO == 0;
}

void Object::translate(const glm::vec3& translate_vect)
{
	model_matrix = glm::translate(model_matrix, translate_vect);
}

void Object::rotate(const glm::vec3& rotation_axis, GLfloat angle_deg)
{
	model_matrix = glm::rotate(model_matrix, glm::radians(angle_deg), glm::normalize(rotation_axis));
}

void Object::scale(const glm::vec3& scale)
{
	model_matrix = glm::scale(model_matrix, scale);
}

void Object::scale(GLfloat scale)
{
	this->scale(glm::vec3(scale));
}

void Object::setModelMatrix()
{
	shader->setMat4("model_matrix", model_matrix);
}

void Object::setColor()
{
	shader->setVec3("our_color.ambient", color.ambient);
	shader->setVec3("our_color.diffuse", color.diffuse);
	shader->setVec3("our_color.specular", color.specular);
}

void Object::setCollisionBox()
{
	// Collision Radius
	//float max_length_xz = 0.0f;
	//for (int i = 0; i < vertices.size(); ++i)
	//{
	//	glm::vec3 no_y_i = glm::vec3(vertices[i].x, 0.0f, vertices[i].z);
	//	float max_i = BaseObject::getLengthVector(no_y_i);//(abs(vertices[i].x) > abs(vertices[i].z)) ? abs(vertices[i].x) : abs(vertices[i].z);
	//	if (max_length_xz < max_i)
	//	{
	//		max_length_xz = max_i;
	//	}
	//}
	//radius = max_length_xz * SCALE * initial_scale_factor;

	// Collision Box
	glm::vec3 min = vertices[0];
	glm::vec3 max = vertices[0];

	for (int i = 1; i < vertices.size(); ++i)
	{
		// Min values
		if (min.x > vertices[i].x)
		{
			min.x = vertices[i].x;
		}
		if (min.z > vertices[i].z)
		{
			min.z = vertices[i].z;
		}
		if (min.y > vertices[i].y)
		{
			min.y = vertices[i].y;
		}

		// Max values
		if (max.x < vertices[i].x)
		{
			max.x = vertices[i].x;
		}
		if (max.z < vertices[i].z)
		{
			max.z = vertices[i].z;
		}
		if (max.y < vertices[i].y)
		{
			max.y = vertices[i].y;
		}
	}
	collisionBox.back  = min.z;
	collisionBox.front = max.z;

	collisionBox.left  = min.x;
	collisionBox.right = max.x;

	collisionBox.bottom = min.y;
	collisionBox.top    = max.y;

	// Scale
	collisionBox.scale(SCALE);
}

// When overriding this, put Object::initState(); at the top
void Object::initState()
{
	model_matrix = glm::mat4(1.0f);
	scale(SCALE);
	scale(initial_scale_factor);
	// color = glm::vec3(0.0f);
}

// If you need to execute this specific code when you override this function,
// call: Object::drawState()
void Object::drawState()
{
	// Set uniforms
	setModelMatrix();
	setColor();
	//TODO: Implement color for object
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

glm::vec3 Object::getPosition() const
{
	glm::vec3 mod_pos = glm::vec3(model_matrix * glm::vec4(position, 1.0f));
	return mod_pos;
}

// Not meant to be overriden
void Object::update()
{
	initState();
	modState();
	drawState();
}

bool Object::loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec2>& out_uvs)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Failed to open file!");
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		// Read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			// EOF = End Of File. QUit the loop.
			/*std::cout << "Finished parsing object" << std::endl;*/
			break;
		}
		// else: parse lineHeader

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y *= -1; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); ++i)
	{
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attibutes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		/*std::cout << "(" << vertex.x << ", " << vertex.y << "," << vertex.z << ") ";
		std::cout << "(" << uv.x << ", " << uv.y << ") ";
		std::cout << "(" << normal.x << ", " << normal.y << "," << normal.z << ") " << std::endl;*/
	}

	return true;
}



PolarBear::PolarBear(Shader* shader, const glm::vec3& m)
	: Object("res/objects/polarbear.obj", shader)
{
	position = m;
	initial_scale_factor = SCALE_POLAR_BEAR;
	collisionBox.scale(initial_scale_factor);
}


PolarBear::~PolarBear()
{
}

void PolarBear::modState()
{
	translate(position);
	if (position.x <= 0.0f)
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		color = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}

Eskimo::Eskimo(Shader* shader, const glm::vec3& m)
	: Object("res/objects/nana.obj", shader)
{
	initial_scale_factor = SCALE_ESKIMO;
	position = m;
	collisionBox.scale(initial_scale_factor);
}

Eskimo::~Eskimo()
{
}

void Eskimo::modState()
{
	translate(position);
	if (position.x <= 0.0f)
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		color = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}

Penguin::Penguin(Shader* shader, const glm::vec3& m)
	: Object("res/objects/penguin.obj", shader)
{
	initial_scale_factor = SCALE_PENGUIN;
	position = m;
	collisionBox.scale(initial_scale_factor);
}


Penguin::~Penguin()
{
}

void Penguin::modState()
{
	translate(position);
	if (position.x <= 0.0f)
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		color = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}

Igloo::Igloo(Shader* shader, const glm::vec3& m)
	: Object("res/objects/igloo_mod.obj", shader)
{
	initial_scale_factor = SCALE_IGLOO;
	position = m;
	collisionBox.scale(initial_scale_factor);
}


Igloo::~Igloo()
{
}

void Igloo::modState()
{
	translate(position);
	if (position.x <= 0.0f)
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		color = glm::vec3(0.0f, 0.0f, 1.0f);
	}
}