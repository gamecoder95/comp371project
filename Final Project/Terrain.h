#pragma once
#include "stdafx.h"
#include "BaseObject.h"
#include "Shader.h"
#include <vector>
#include "Chunk.h"

using namespace std;

class Terrain : public BaseObject
{
private:
	const static int SIZE = 3;

	vector<vector<Chunk>> visibleChunk;
	glm::vec3 playerPos;
	
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;

public:
	Terrain(Shader *sh);
	~Terrain();

	void setPlayerPos(glm::vec3 pos);
	glm::vec3 getPlayerPos();

	void update();
	bool isDestroyed();
	void destroy();


};
