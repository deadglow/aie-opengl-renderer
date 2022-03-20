#pragma once
#include "Mesh.h"
#include <string>

class MeshLoadFunctions
{
public:


	static Mesh* CreateMeshFromFile(const std::string filename);
};

