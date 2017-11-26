#include "stdafx.h"
#include "Terrain.h"

// ---------------------------------- Chunk -----------------------------------

const glm::mat4 BASIS(	-0.5f, 1.5f, -1.5f, 0.5f,
						1.0f, -2.5f, 2.0f, -0.5f,
						-0.5f, 0.0f, 0.5f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f );

const float Chunk::NEAR_FRACT = 0.3;
const float Chunk::FAR_FRACT = 0.7;


Chunk::Chunk() {}

Chunk::Chunk(int ox, int oz, float s, Shader* shader)
{

	offsetX = ox;
	offsetZ = oz;
	seed = s;
	float rng = seed + offsetX * 2 + offsetZ / 2;
	srand(seed);
	for (int i = 0; i <= SIZE; i++)
	{
		vector<glm::vec3> temp;
		for (int j = 0; j <= SIZE; j++)
		{
			glm::vec3 coordinate(i + 10 * ox, 0, j + 10 * oz);
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

	//TODO figure out exactly how to run interpolation... needs two passes
	//interpolate(grid, FAR_FRACT);	//TODO figure out a good way of deciding if interpolation is to be done with NEAR_FRACT or FAR_FRACT

	//draw setup
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, interpolated.size() * sizeof(glm::vec3), &interpolated.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("position"));// Replace with non-hard-coded value

	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("in_normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("in_normal"));// Replace with non-hard-coded value

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//TODO collision box


}

float Chunk::getHeightAt(float x, float z)
{
	return grid[x][z].y;	//TODO needs fixing
}

vector<int> Chunk::generateEBO(){
	std::vector<int> indices;

	for (int i = 0; i < interpolated.size() - 1; i++){
		for (int k = 0; k < interpolated[i].size() - 1; k++){
			indices.push_back(k + i*interpolated[i].size());								//first point
			indices.push_back(k + i*interpolated[i].size() + 1);							//point next to it
			indices.push_back(k + i*interpolated[i].size() + interpolated[i].size());		//point under first point
			indices.push_back(k + i*interpolated[i].size() + 1);							//point next to first point 
			indices.push_back(k + i*interpolated[i].size() + interpolated[i].size());		//point under first point
			indices.push_back(k + i*interpolated[i].size() + interpolated[i].size() + 1);	//point next to point below first point
		}
	}
	return indices;
}

vector<glm::vec3> Chunk::getInterpolatedPoints(){
	return flatten();
}

vector<glm::vec3> Chunk::flatten()
{
	vector<glm::vec3> temp;

	for (int i = 0; i < interpolated.size(); i++){
		for (int j = 0; j < interpolated[i].size(); j++){
			//lines each row next to each other
			temp.push_back(interpolated[i][j]);
		}
	}

	return temp;
} 

//interpolation function, creates control matrix and sends it into the catmull-rom function for every point
vector<vector<glm::vec3>> Chunk::interpolate(vector<vector<glm::vec3>> array, float fraction){
	vector<vector<glm::vec3>>output;


	for (int i = 0; i < array.size(); i++){
		vector<glm::vec3> temp;
		for (int j = 1; j < array[i].size() - 2; j++){
			//gets necessary points
			glm::vec3 p = array[i][j - 1];
			glm::vec3 p2 = array[i][j];
			glm::vec3 p3 = array[i][j + 1];
			glm::vec3 p4 = array[i][j + 2];

			//creates control matrix
			glm::mat4x3 control(p[0], p[1], p[2],
				p2[0], p2[1], p2[2],
				p3[0], p3[1], p3[2],
				p4[0], p4[1], p4[2]);

			//CATMULL-ROM 
			vector<glm::vec3> cat = catmull_rom(control, fraction);

			//adds interpolated new values to the row  
			for (int k = 0; k < cat.size(); k++){
				temp.push_back(cat[k]);
			}
		}

		//adds the row to the map
		output.push_back(temp);
	}

	interpolated = output;
}

//actual catmull-rom interpolation between 2 points
vector<glm::vec3> Chunk::catmull_rom(glm::mat4x3 control, float fraction){

	vector<glm::vec3> new_points;
	for (float u = 0; u < 1.0f; u += fraction){
		//creates u vector
		glm::vec4 u_vector((u*u*u), (u*u), u, 1.0f);

		//generates new point 
		glm::vec3 point = control*BASIS*u_vector;

		//sends point back to calling function 
		new_points.push_back(point);
	}
	return new_points;

}

void Chunk::update() {	//TODO
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, interpolated.size());
	glBindVertexArray(0);
}

// -------------------------------------- Terrain --------------------------------------

Terrain::Terrain(Shader* sh) : BaseObject(sh){

	visibleChunk = vector<vector<Chunk>>();

	//generates starting chunks
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			visibleChunk[i][j] = Chunk(i - 1, j - 1, 0, sh);
		}
	}


}

void Terrain::update(){
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			visibleChunk[i][j].update();
		}
	}
}

void Terrain::destroy()
{
	//TODO figure this out
}

bool Terrain::isDestroyed()
{
	return false;		//think about this a bit
}

