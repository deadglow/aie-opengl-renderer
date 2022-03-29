#pragma once
#include "Vertex.h"
#include "Triangle.h"
#include "Material.h"
#include <vector>
#include <string>

class Mesh
{
private:
	std::string filename;
	bool loaded = false;
	unsigned int VAO, VBO, EBO;

public:
	glm::mat4 transform;
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	Mesh(std::string filename_init, glm::mat4 transform_init, std::vector<Vertex> vertices_init, std::vector<Triangle> triangles_init);
	~Mesh();

	const bool GetLoaded() const { return loaded; }
	void LoadMesh();
	void UnloadMesh();
	void Draw() const;
};