#pragma once
#include "stdafx.h"
#include "shader.h"
#include <vector>

using namespace std;

class Chunk
{
private:
	vector<vector<glm::vec3>> grid;
	vector<vector<glm::vec3>> interpolated;	//TODO try and work out a way to have a 1D array
	vector<glm::vec3> normals;
	vector<int> order;

	int offsetX;
	int offsetZ;
	float seed;

	float curr_fract;

	// Catmull-rom basis matrix
	const static glm::mat4 BASIS;
	const static float NEAR_FRACT;	//for some reason float initialization must be done in constructor
	const static float FAR_FRACT;

	//draw stuff
	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_normals;
	GLuint EBO; // Find out what to do with this

	glm::mat4 model_matrix;
	Shader* shader;

	vector<glm::vec3> flatten();
	vector<vector<glm::vec3>> flip(vector<vector<glm::vec3>>);
	void normalizePoints();
	void generateEBO();

	vector<vector<glm::vec3>> interpolate(vector<vector<glm::vec3>> grid, float fraction);
	vector<glm::vec3> catmull_rom(glm::mat4x3 control, float fraction);

	bool isPointInTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p);

	float randomFloat(float a, float b);

public:
	const static int SIZE = 10;

	Chunk();
	Chunk(int ox, int oz, float s, Shader* sh);

	float getHeightAt(float x, float z);
	int getOffsetX();
	int getOffsetZ();

	void update();

};
