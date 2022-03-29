#pragma once
#include "Texture.h"

class Cubemap :
	public Texture
{
private:

public:
	Cubemap(GLuint id_init, const std::string filename_init);
	~Cubemap();


	void UpdateTextureProperties();
	TEX_Type GetTexType();
};

