#include "Mesh.h"
#include "glad.h"
#include "glfw3.h"
#include <iostream>

void Mesh::LoadMesh()
{
	if (loaded)
	{
		std::cout << "Mesh already loaded." << std::endl;
		return;
	}

	// initialise buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// pass in vertex data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// pass in indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), & indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	// vertex colors
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);

	loaded = true;
}

void Mesh::UnloadMesh()
{
	if (!loaded)
	{
		std::cout << "Mesh not loaded." << std::endl;
		return;
	}
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);

	loaded = false;
}

Mesh::Mesh(std::string filename_init, std::vector<Vertex> vertices_init, std::vector<unsigned int> indices_init)
{
	filename = filename_init;
	vertices = vertices_init;
	indices = indices_init;
}

Mesh::~Mesh()
{
	UnloadMesh();
}

void Mesh::Draw(ShaderConfiguration* shaderConfig) const
{
	shaderConfig->UseShader();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
