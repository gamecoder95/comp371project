#ifndef LIGHT_H
#define LIGHT_H

#include "Shader.h"
#include "BaseObject.h"

// This struct holds all of the information pertaining to the color of the object
struct Color
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	//float shininess;

	// Contructor to initialize member variables with default arguments
	Color(const glm::vec3& amb = glm::vec3(0.1f), const glm::vec3& diff = glm::vec3(1.0f), const glm::vec3& spec = glm::vec3(1.0f)/*, float shin = 0.0f*/)
	{
		ambient = amb;
		diffuse = diff;
		specular = spec;
		//shininess = shin;
	}

	// Overload assignment operator
	Color& operator=(const Color& other)
	{

		ambient = other.ambient;
		diffuse = other.diffuse;
		specular = other.specular;
		//shininess = other.shininess;
		return *this;
	}
};

class DirectionalLight : public BaseObject
{
private: 
	glm::vec3 direction;
	glm::vec3 position;
	Color color;

public:
	DirectionalLight(Shader* shader, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& dir = glm::vec3(-1.0f), const Color& col = Color());
	void setColor(const Color& col);
	void setPosition(const glm::vec3& pos); // Change position to have transformations possible?
	void setDirection(const glm::vec3& dir);

	Color getColor() const;
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;
	
	~DirectionalLight();
};

#endif;