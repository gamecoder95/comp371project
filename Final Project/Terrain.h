#include "stdafx.h"
#include "BaseObject.h"
#include "Shader.h"
#include <vector>

using namespace std;

class Terrain : BaseObject
{
private:
	const static int SIZE = 3;

	vector<vector<Chunk>> visibleChunk;
	
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;

	glm::mat4 model_matrix;

public:
	Terrain(Shader *sh);
	~Terrain();

	void update();
	bool isDestroyed();
	void destroy();


};

class Chunk
{
private:
	vector<vector<glm::vec3>> grid;
	vector<vector<glm::vec3>> interpolated;	//TODO try and work out a way to have a 1D array
	vector<glm::vec3> normals;
	
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

	vector<glm::vec3> flatten();

public:
	Chunk();
	Chunk(int ox, int oz, float s, Shader* shader);

	float getHeightAt(float x, float z);
	vector<glm::vec3> getInterpolatedPoints();

	vector<vector<glm::vec3>> interpolate(vector<vector<glm::vec3>> grid, float fraction);
	vector<glm::vec3> catmull_rom(glm::mat4x3 control, float fraction);
	vector<int> generateEBO();

	void update();

};
