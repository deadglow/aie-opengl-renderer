#include "Texture.h"

Texture::Texture(unsigned char* data, const int width_init, const int height_init, const int nrChannels_init, const std::string filename_init)
{
    glGenTextures(1, &id);
    width = width_init;
    height = height_init;
    filename = filename_init;

    switch (nrChannels_init)
    {
    case 1:
        format = TEX_Format::R;
        break;
    case 2:
        format = TEX_Format::RG;
        break;
    case 3:
        format = TEX_Format::RGB;
        break;
    case 4:
        format = TEX_Format::RGBA;
        break;
    }

    UpdateTexture();
    // assign texture data
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
    if (IsLoaded())
    {
        glDeleteTextures(1, &id);
    }
}

const bool Texture::IsLoaded() const
{
    return id != -1;
}

const GLuint Texture::GetID() const
{
    return id;
}

const int Texture::GetWidth() const
{
    return width;
}

const int Texture::GetHeight() const
{
    return height;
}

const std::string Texture::GetFilename() const
{
    return filename;
}

void Texture::SetWrapMode(TEX_WrapMode s, TEX_WrapMode t)
{
    wrapMode[0] = s;
    wrapMode[1] = t;
}

void Texture::SetMipMapFilter(TEX_MipMapFiltering filter)
{
    minFilter = filter;
}

void Texture::SetFilter(TEX_Filtering filter)
{
    magFilter = filter;
}

void Texture::UpdateTexture()
{
    glBindTexture(GL_TEXTURE_2D, id);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)magFilter);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}
