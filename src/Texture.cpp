#include "Texture.h"
#include "TextureLoader.h"

Texture::Texture(const std::string filename_init)
{
	filename = filename_init;
}

Texture::~Texture()
{
	if (IsLoaded())
	{
		Unload();
	}
}

void Texture::Load()
{
	TextureLoader::LoadTexture(this);
}

void Texture::Unload()
{
	TextureLoader::UnloadTexture(this);
}

const bool Texture::IsLoaded() const
{
	return id != -1;
}

void Texture::SetID(GLuint newid)
{
	id = newid;
}

const GLuint Texture::GetID() const
{
	return id;
}

const std::string Texture::GetFilename() const
{
	return filename;
}

void Texture::SetMinFilter(TEX_Filtering filter)
{
	minFilter = filter;
}

void Texture::SetMagFilter(TEX_Filtering filter)
{
	magFilter = filter;
}

void Texture::UpdateTextureProperties()
{
	glBindTexture(GL_TEXTURE_2D, id);

	// set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)magFilter);
}

TEX_Type Texture::GetTexType()
{
	return TEX_Type();
}


TEX_Format Texture::CalculateFormat(const int numChannels)
{
	switch (numChannels)
	{
	case 1:
		return TEX_Format::R;
		break;
	case 2:
		return TEX_Format::RG;
		break;
	case 3:
		return TEX_Format::RGB;
		break;
	case 4:
		return TEX_Format::RGBA;
		break;
	}

	return TEX_Format::INVALID;
}
