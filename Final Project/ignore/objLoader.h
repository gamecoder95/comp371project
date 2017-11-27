#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <glm.hpp>

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec2>& out_uvs
);

#endif // !OBJ_LOADER_H

