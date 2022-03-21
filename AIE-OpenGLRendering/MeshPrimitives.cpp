#include "MeshPrimitives.h"

Mesh* MeshPrimitives::CreateCube(float width)
{
	// front, right, back, left, up, down
	glm::vec3 normals[6]
	{
		{0, 0, 1},
		{1, 0, 0},
		{0, 0, -1},
		{-1, 0, 0},
		{0, 1, 0},
		{0, -1, 0}
	};
	// bottom left, bottom right, top left, top right
	glm::vec2 texCoords[4]
	{
		{0, 1},
		{1, 1},
		{0, 0},
		{1, 0}
	};

	// help
	Vertex vertices[36]
	{
		// front -
		{{-1, -1, 1}, normals[0], texCoords[0]},
		{{1, -1, 1}, normals[0], texCoords[1]},
		{{-1, 1, 1}, normals[0], texCoords[2]},
		{{1, -1, 1}, normals[0], texCoords[1]},
		{{1, 1, 1}, normals[0], texCoords[3]},
		{{-1, 1, 1}, normals[0], texCoords[2]},
		// right -
		{{1, -1, 1}, normals[1], texCoords[0]},
		{{1, -1, -1}, normals[1], texCoords[1]},
		{{1, 1, 1}, normals[1], texCoords[2]},
		{{1, -1, -1}, normals[1], texCoords[1]},
		{{1, 1, -1}, normals[1], texCoords[3]},
		{{1, 1, 1}, normals[1], texCoords[2]},
		// back -
		{{1, -1, -1}, normals[2], texCoords[0]},
		{{-1, -1, -1}, normals[2], texCoords[1]},
		{{1, 1, -1}, normals[2], texCoords[2]},
		{{-1, -1, -1}, normals[2], texCoords[1]},
		{{-1, 1, -1}, normals[2], texCoords[3]},
		{{1, 1, -1}, normals[2], texCoords[2]},
		// left -
		{{-1, -1, -1}, normals[3], texCoords[0]},
		{{-1, -1, 1}, normals[3], texCoords[1]},
		{{-1, 1, -1}, normals[3], texCoords[2]},
		{{-1, -1, 1}, normals[3], texCoords[1]},
		{{-1, 1, 1}, normals[3], texCoords[3]},
		{{-1, 1, -1}, normals[3], texCoords[2]},
		// up -
		{{-1, 1, 1}, normals[4], texCoords[0]},
		{{1, 1, 1}, normals[4], texCoords[1]},
		{{-1, 1, -1}, normals[4], texCoords[2]},
		{{1, 1, 1}, normals[4], texCoords[1]},
		{{1, 1, -1}, normals[4], texCoords[3]},
		{{-1, 1, -1}, normals[4], texCoords[2]},
		// down -
		{{-1, -1, -1}, normals[5], texCoords[0]},
		{{1, -1, -1}, normals[5], texCoords[1]},
		{{-1, -1, 1}, normals[5], texCoords[2]},
		{{1, -1, -1}, normals[5], texCoords[1]},
		{{1, -1, 1}, normals[5], texCoords[3]},
		{{-1, -1, 1}, normals[5], texCoords[2]},
	};
	
	// scale the positions
	for (int i = 0; i < 36; ++i)
	{
		vertices[i].pos *= width * 0.5f;
	}

	// initialise vectors
	std::vector<Vertex> verticesList;
	verticesList.resize(36);
	std::vector<Triangle> triangles;
	triangles.resize(12);

	// assign vertices
	memcpy_s(&verticesList[0], sizeof(vertices), vertices, sizeof(vertices));
	// generate triangle indexes
	for (int i = 0; i < 6; ++i)
	{
		unsigned int o = i * 6;
		Triangle tris[2]
		{
			{o + 0u, o + 1u, o + 2u},
			{o + 3u, o + 4u, o + 5u}
		};
		triangles[i * 2] = tris[0];
		triangles[i * 2 + 1] = tris[1];
	}

	// vertices and indices complete
	Mesh* box = new Mesh("Primitive_Box", verticesList, triangles, 1.0f);
	return box;
}

Mesh* MeshPrimitives::CreateSphere(float radius)
{
	return nullptr;
}

Mesh* MeshPrimitives::CreateCylinder(float radius, float height)
{
	return nullptr;
}

Mesh* MeshPrimitives::CreateSausage(float radius, float height)
{
	return nullptr;
}
