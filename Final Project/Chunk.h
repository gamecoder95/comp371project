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

	// Catmull-rom basis matrix
	const static glm::mat4 BASIS;
	const static int SIZE = 10;
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

public:
	Chunk();
	Chunk(int ox, int oz, float s, Shader* sh);

	float getHeightAt(float x, float z);

	void update();

};
