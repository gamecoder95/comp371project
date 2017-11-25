#include "stdafx.h"
#include <vector>
#include "glm.hpp"
using namespace std;
class Terrain
{
private:
	vector<vector<glm::vec3>> grid;
	int offsetX;
	int offsetZ;
	float seed;
public:
	Terrain();
	Terrain(int ox, int oz, float s);
	vector<glm::vec3> getRow(int col);
	float getHeight(int x, int z);
	vector<vector<glm::vec3>> getGrid();
	vector<float> flatten();
	
};