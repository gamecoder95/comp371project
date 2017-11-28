#include "stdafx.h"
#include "Terrain.h"
#include <iostream>

// -------------------------------------- Terrain --------------------------------------
Terrain::~Terrain(){
	destroy();
}

Terrain::Terrain(Shader* sh) : BaseObject(sh){

	visibleChunk = vector<vector<Chunk>>();

	//generates starting chunks
	for (int i = 0; i < SIZE; i++){
		vector<Chunk> temp = vector<Chunk>();
		for (int j = 0; j < SIZE; j++){
			temp.push_back(Chunk(i - 1, j - 1, 5, sh));	//todo add random gen here
		}

		visibleChunk.push_back(temp);
	}


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

