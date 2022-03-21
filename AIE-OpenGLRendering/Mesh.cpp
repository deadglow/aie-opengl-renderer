#include "Mesh.h"
#include "glad.h"
#include "glfw3.h"
#include <iostream>

Mesh::Mesh(std::string filename_init, std::vector<Vertex> vertices_init, std::vector<Triangle> triangles_init, float unitScale_init)
{
	filename = filename_init;
	vertices = vertices_init;
	triangles = triangles_init;
	unitScale = unitScale_init;
}

Mesh::~Mesh()
{
	UnloadMesh();
}

void Mesh::Draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (int)triangles.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

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

	// pass in vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// pass in indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0], GL_STATIC_DRAW);

	// interpret vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// interpret vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	// interpret vertex texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

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