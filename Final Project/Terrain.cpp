#include "stdafx.h"
#include "Terrain.h"
#include "Person.h"
#include <iostream>

// -------------------------------------- Terrain --------------------------------------

Terrain::~Terrain(){
	destroy();
}

Terrain::Terrain(Shader* sh) : BaseObject(sh, "Terrain"){

	visibleChunk = vector<vector<Chunk>>();

	//generates starting chunks
	for (int i = 0; i < SIZE; i++){
		vector<Chunk> temp = vector<Chunk>();
		for (int j = 0; j < SIZE; j++){
			temp.push_back(Chunk(i - 1, j - 1, 5, sh));	//todo add random gen here
		}

		visibleChunk.push_back(temp);
	}

	setCollisionBox();
}

float Terrain::getHeightAt(float x, float z){
	int chunkx = floor(x / visibleChunk[0][0].SIZE);
	int chunkz = floor(z / visibleChunk[0][0].SIZE);
	cout << "Chunk Coords: " << chunkx << " " << chunkz << endl;
	
	
	for (int i = 0; i < visibleChunk.size(); i++){
		for (int j = 0; j < visibleChunk[i].size(); j++){
			if (visibleChunk[i][j].getOffsetX() == chunkx && visibleChunk[i][j].getOffsetZ() == chunkz){
				return visibleChunk[i][j].getHeightAt(x, z);
			}
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

void Terrain::setPlayerPos(glm::vec3 pos){
	playerPos = pos;

}


glm::vec3 Terrain::getPlayerPos(){

	return playerPos;
}

void Terrain::setCollisionBox(){
	collisionBox.back = visibleChunk[0][0].SIZE / 2.0f;
	collisionBox.front = visibleChunk[0][0].SIZE / 2.0f;
	collisionBox.left = visibleChunk[0][0].SIZE / 2.0f;
	collisionBox.right = visibleChunk[0][0].SIZE / 2.0f;
	collisionBox.top = INT_MAX;
	collisionBox.bottom = INT_MAX;
}

void Terrain::onCollision(BaseObject& other){
	cout << "Terrain collision detected" << endl;
	if (dynamic_cast<Person*>(&other) != nullptr && isCollision(other)){
		//TODO add new chunk generation
		cout << "Player collision with New chunk border" << endl;
	}
}

bool Terrain::isCollision(const BaseObject& other){
	if (other.getBack() < this->getFront()
		|| other.getFront() > this->getBack()
		|| other.getRight() > this->getLeft()
		|| other.getLeft() < this->getRight()
		|| other.getTop() > this->getBottom()
		|| other.getBottom() < this->getTop())
	{
		return false;
	}
	else
	{
		return true;
	}
}



