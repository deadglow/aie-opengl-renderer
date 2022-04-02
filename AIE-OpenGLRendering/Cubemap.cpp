#include "Cubemap.h"


Cubemap::Cubemap(const std::string filename_init) : Texture(filename_init)
{
}

Cubemap::~Cubemap()
{

}

void Cubemap::UpdateTextureProperties()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	// set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLenum)magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLenum)magFilter);

	// unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TEX_Type Cubemap::GetTexType()
{
	return TEX_Type::Cubemap;
}
