#include "stdafx.h"
#include "Terrain.h"

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
			temp.push_back(Chunk(i - 1, j - 1, 1, sh));
		}

		visibleChunk.push_back(temp);
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

