#include "stdafx.h"
#include "Chunk.h"
#include <iostream>
#include "math.h"


// ---------------------------------- Chunk -----------------------------------

const glm::mat4 Chunk::BASIS( -0.5f, 1.5f, -1.5f, 0.5f,
1.0f, -2.5f, 2.0f, -0.5f,
-0.5f, 0.0f, 0.5f, 0.0f,
0.0f, 1.0f, 0.0f, 0.0f );

const float Chunk::NEAR_FRACT = 0.3;
const float Chunk::FAR_FRACT = 0.7;


Chunk::Chunk() {}

Chunk::Chunk(int ox, int oz, float s, Shader* sh)
{
	//sets up the basic model matrix and shader object
	model_matrix = glm::mat4(1.0f);
	shader = sh;

	offsetX = ox;
	offsetZ = oz;
	seed = s;
	float rng = seed + offsetX * 2 + offsetZ / 2;
	srand(rng);
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
			if (i != 0 && i != 10 && j != 0 && j != 10)
			{
				grid[i][j].y = randomFloat(-0.05, 0.25);
			}
		}
	}

	//TODO figure out exactly how to run interpolation... needs two passes
	interpolated = interpolate(grid, FAR_FRACT);
	interpolated = interpolate(flip(interpolated), FAR_FRACT);
	curr_fract = FAR_FRACT;	//todo temporary
	//interpolate(grid, FAR_FRACT);	//TODO figure out a good way of deciding if interpolation is to be done with NEAR_FRACT or FAR_FRACT

	//draw setup
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vector<glm::vec3> flattened = flatten();
	glGenBuffers(1, &VBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, flattened.size() * sizeof(glm::vec3), &flattened.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("position"));// Replace with non-hard-coded value

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	normalizePoints();
	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(shader->getAttribLoc("in_normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(shader->getAttribLoc("in_normal"));// Replace with non-hard-coded value

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	generateEBO();
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*order.size(), &order.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//TODO collision box

}


float Chunk::getHeightAt(float x, float z)
{

	//find related points
	//TODO need to fix
	int xCoord = abs(fmod(x / curr_fract, interpolated.size()));
	int zCoord = abs(fmod(z / curr_fract, interpolated.size()));

	cout << xCoord << " " << zCoord << endl;
	cout << x << " " << interpolated.size() << endl;
	cout << z << " " << interpolated.front().size() << endl;

	glm::vec3 p1 = interpolated[xCoord][zCoord];
	glm::vec3 p2;
	glm::vec3 p3;

	for (int i = 0; i < 2; i++){

		if (xCoord + i - 1 < 0 || xCoord + i + 1 > interpolated.size()){
			continue;
		}

		for (int j = 0; j < 2; j++){

			if (zCoord + j - 1 < 0 || zCoord + j + 1 > interpolated.front().size()){
				continue;
			}

			//middle point is always part of the triangle
			if (i == 0 && j == 0){
				continue;
			}
			
			p2 = interpolated[xCoord + i - 1][zCoord + j - 1];
			
			if (i == 1){
				p3 = interpolated[xCoord + i - 1][zCoord + j];
			}
			else{
				p3 = interpolated[xCoord + i][zCoord + j + 1];
			}
			
			//check if point is inside this triangle
			if (isPointInTriangle(p1, p2, p3, glm::vec3(x, 0, z))){
				break;
			}


		}
	}

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;

	glm::vec3 n = glm::cross(v1, v2);
	float d = glm::dot(n, v1);
	return (d - n.x*v1.x - n.z *v1.z) / n.y;
}

//stolen directly from http://blackpawn.com/texts/pointinpoly/
bool Chunk::isPointInTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 p){

	// Compute vectors        
	glm::vec3 v0 = c - a;
	glm::vec3 v1 = b - a;
	glm::vec3 v2 = p - a;

	// Compute dot products
	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);
	
	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);
}

void Chunk::generateEBO(){
	for (int i = 0; i < interpolated.size() - 1; i++){
		for (int k = 0; k < interpolated[i].size() - 1; k++){
			order.push_back(k + i*interpolated[i].size());								//first point
			order.push_back(k + i*interpolated[i].size() + 1);							//point next to it
			order.push_back(k + i*interpolated[i].size() + interpolated[i].size());		//point under first point
			order.push_back(k + i*interpolated[i].size() + 1);							//point next to first point 
			order.push_back(k + i*interpolated[i].size() + interpolated[i].size());		//point under first point
			order.push_back(k + i*interpolated[i].size() + interpolated[i].size() + 1);	//point next to point below first point
		}
	}
}

int Chunk::getOffsetX(){
	return offsetX;
}

int Chunk::getOffsetZ(){
	return offsetZ;
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

vector<vector<glm::vec3>> Chunk::flip(vector<vector<glm::vec3>> input){
	vector<vector<glm::vec3>>* temp = new vector<vector<glm::vec3>>();
	//flip map
	int height = input.size();
	int width = input[0].size();
	for (int i = 0; i < width; i++){
		vector<glm::vec3> tempRow;
		for (int j = 0; j < height; j++){
			//adds inverted column value to row
			tempRow.push_back(input[j][i]);
		}
		//adds row to map
		temp->push_back(tempRow);
	}

	return *temp;
}

//interpolation function, creates control matrix and sends it into the catmull-rom function for every point
vector<vector<glm::vec3>> Chunk::interpolate(vector<vector<glm::vec3>> array, float fraction){
	vector<vector<glm::vec3>>output;


	for (int i = 0; i < array.size(); i++){
		vector<glm::vec3> temp;
		for (int j = 0; j < array[i].size(); j++){
			//gets necessary points
			glm::vec3 p;
			glm::vec3 p2;
			glm::vec3 p3;
			glm::vec3 p4;

			p2 = array[i][j];

			if (j < 1){
				p = array[i][j];
			}
			else{
				p = array[i][j - 1];
			}

			if (j >= array[i].size() - 2){
				p3 = array[i][j];
				p4 = array[i][j];
			}
			else{
				p3 = array[i][j + 1];
				p4 = array[i][j + 2];
			}

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

	return output;
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

void Chunk::normalizePoints(){
	for (int i = 0; i < interpolated.size() - 1; i++){
		for (int j = 0; j < interpolated[i].size() - 1; j++){
			glm::vec3 p1 = interpolated[i][j];
			glm::vec3 p2 = interpolated[i + 1][j];
			glm::vec3 p3 = interpolated[i][j + 1];

			glm::vec3 v1 = p2 - p1;
			glm::vec3 v2 = p3 - p1;

			glm::vec3 norm = glm::cross(v1, v2);
			for (int i = 0; i < 3; i++){
				normals.push_back(norm);
			}

			p1 = interpolated[i + 1][j];
			p2 = interpolated[i + 1][j + 1];
			p3 = interpolated[i][j + 1];

			v1 = p1 - p2;
			v2 = p3 - p2;

			norm = glm::cross(v1, v2);
			for (int i = 0; i < 3; i++){
				normals.push_back(norm);
			}
		}
	}
}

void Chunk::update() {
	shader->setMat4("model_matrix", model_matrix);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, order.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

float Chunk::randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}