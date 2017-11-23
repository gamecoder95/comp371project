#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain() {}

Terrain::Terrain(int ox, int oz, float s)
{

	offsetX = ox;
	offsetZ = oz;
	seed = s;
	float rng = seed + offsetX + offsetZ;
	srand(seed);
	for (int x = 0; x <10; x++)
	{
		for (int z = 0; z <= 11; z++)
		{
			glm::vec3 coordinate(x+offsetX, 0, z+offsetZ);
			grid[x][z] = coordinate;
		}
	}
	for (int i = 0; i < grid.size; i++)
	{
		for (int j = 0; j < grid[i].size; j++)
		{
			grid[i][j].y = (rand()%13) - 6;
		}
	}
}