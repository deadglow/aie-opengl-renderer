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
	unsigned int VAO, VBO, EBO;

	void UnloadMesh();
	void LoadMesh();
public:
	glm::mat4 transform;
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	Mesh(std::string filename_init, glm::mat4 transform_init, std::vector<Vertex> vertices_init, std::vector<Triangle> triangles_init);
	~Mesh();

	void Draw() const;
};