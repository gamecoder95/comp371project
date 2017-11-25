#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain() {}

Terrain::Terrain(int ox, int oz, float s)
{

	offsetX = ox;
	offsetZ = oz;
	seed = s;
	float rng = seed + offsetX*2 + offsetZ/2;
	srand(seed);
	for (int i = 0; i < 11; i++)
	{
		vector<glm::vec3> temp;
		for (int j = 0; j < 11; j++)
		{
			glm::vec3 coordinate(i+10*ox, 0, j+10*oz);
			temp.push_back(coordinate);
		}
		grid.push_back(temp);
	}
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			grid[i][j].y = -5 + (rand() % (6));
		}
	}


}

float Terrain::getHeight(int x, int z)
{
	return grid[x][z].y;
}
vector<glm::vec3> Terrain::getRow(int col)
{
	return grid[col];
}
vector<vector<glm::vec3>> Terrain::getGrid()
{
	return grid;
}
vector<float> Terrain::flatten()
{
	vector<float> g;
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			g.push_back(grid[i][j].x);
			g.push_back(grid[i][j].y);
			g.push_back(grid[i][j].z);
		}
	}
	return g;
}
