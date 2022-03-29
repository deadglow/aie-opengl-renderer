#include "Texture.h"

Texture::Texture(const GLuint id_init, const std::string filename)
{
	id = id_init;
}

Texture::~Texture()
{
}

const bool Texture::IsLoaded() const
{
	return id != -1;
}

const GLuint Texture::GetID() const
{
	return id;
}

const std::string Texture::GetFilename() const
{
	return filename;
}

void Texture::SetMipMapFilter(TEX_MipMapFiltering filter)
{
	minFilter = filter;
}

void Texture::SetFilter(TEX_Filtering filter)
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
