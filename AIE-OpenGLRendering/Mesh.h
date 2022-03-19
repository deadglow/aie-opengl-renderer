#pragma once
#include "Vertex.h"
#include "ShaderConfiguration.h"
#include <vector>
#include <string>

class Mesh
{
private:
	std::string filename;
	bool loaded = false;
	unsigned int VAO, VBO, EBO;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::string filename_init, std::vector<Vertex> vertices_init, std::vector<unsigned int> indices_init);
	~Mesh();

	const bool GetLoaded() const { return loaded; }
	void LoadMesh();
	void UnloadMesh();
	void Draw(ShaderConfiguration* shaderConfig) const;
};