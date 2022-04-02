#pragma once
#include "Texture.h"

class Cubemap :
	public Texture
{
private:

public:
	Cubemap(const std::string filename_init);
	~Cubemap();

	void UpdateTextureProperties();
	TEX_Type GetTexType();
};

