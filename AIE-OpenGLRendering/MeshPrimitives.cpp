#include "MeshPrimitives.h"

Mesh* MeshPrimitives::CreateCube(float width, float height, float depth)
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
	glm::vec3 tangents[6]
	{
		{1, 0, 0},
		{0, 0, 1},
		{-1, 0, 0},
		{0, 0, -1},
		{1, 0, 0},
		{-1, 0, 0}
	};

	// bottom left, bottom right, top left, top right
	glm::vec2 texCoords[4]
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	};

	// help
	Vertex vertices[36]
	{
		// front -
		{{-1, -1, -1},	normals[0], tangents[0], texCoords[0]},
		{{1, -1, -1},	normals[0], tangents[0], texCoords[1]},
		{{-1, 1, -1},	normals[0], tangents[0], texCoords[2]},
		{{1, -1, -1},	normals[0], tangents[0], texCoords[1]},
		{{1, 1, -1},	normals[0], tangents[0], texCoords[3]},
		{{-1, 1, -1},	normals[0], tangents[0], texCoords[2]},
		// right -				     
		{{1, -1, -1},	normals[1], tangents[1], texCoords[0]},
		{{1, -1, 1},	normals[1], tangents[1], texCoords[1]},
		{{1, 1, -1},	normals[1], tangents[1], texCoords[2]},
		{{1, -1, 1},	normals[1], tangents[1], texCoords[1]},
		{{1, 1, 1},		normals[1], tangents[1], texCoords[3]},
		{{1, 1, -1},	normals[1], tangents[1], texCoords[2]},
		// back -				    
		{{1, -1, 1},	normals[2], tangents[2], texCoords[0]},
		{{-1, -1, 1},	normals[2], tangents[2], texCoords[1]},
		{{1, 1, 1},		normals[2], tangents[2], texCoords[2]},
		{{-1, -1, 1},	normals[2], tangents[2], texCoords[1]},
		{{-1, 1, 1},	normals[2], tangents[2], texCoords[3]},
		{{1, 1, 1},		normals[2], tangents[2], texCoords[2]},
		// left -				     
		{{-1, -1, 1},	normals[3], tangents[3], texCoords[0]},
		{{-1, -1, -1},	normals[3], tangents[3], texCoords[1]},
		{{-1, 1, 1},	normals[3], tangents[3], texCoords[2]},
		{{-1, -1, -1},	normals[3], tangents[3], texCoords[1]},
		{{-1, 1, -1},	normals[3], tangents[3], texCoords[3]},
		{{-1, 1, 1},	normals[3], tangents[3], texCoords[2]},
		// up -					    
		{{-1, 1, -1},	normals[4], tangents[4], texCoords[0]},
		{{1, 1, -1},	normals[4], tangents[4], texCoords[1]},
		{{-1, 1, 1},	normals[4], tangents[4], texCoords[2]},
		{{1, 1, -1},	normals[4], tangents[4], texCoords[1]},
		{{1, 1, 1},		normals[4], tangents[4], texCoords[3]},
		{{-1, 1, 1},	normals[4], tangents[4], texCoords[2]},
		// down -				    
		{{-1, -1, 1},	normals[5], tangents[5], texCoords[0]},
		{{1, -1, 1},	normals[5], tangents[5], texCoords[1]},
		{{-1, -1, -1},	normals[5], tangents[5], texCoords[2]},
		{{1, -1, 1},	normals[5], tangents[5], texCoords[1]},
		{{1, -1, -1},	normals[5], tangents[5], texCoords[3]},
		{{-1, -1, -1},	normals[5], tangents[5], texCoords[2]},
	};
	
	// scale the positions
	for (int i = 0; i < 36; ++i)
	{
		vertices[i].pos.x *= width * 0.5f;
		vertices[i].pos.y *= height * 0.5f;
		vertices[i].pos.z *= depth * 0.5f;
		// flip z
		vertices[i].pos.z *= -1;
		vertices[i].normal.z *= -1;
		vertices[i].tangent.z *= -1;
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
	Mesh* box = new Mesh("Primitive_Box", glm::mat4(1.0f), verticesList, triangles);
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
