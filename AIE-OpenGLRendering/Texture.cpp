#include "Texture.h"

Texture2D::Texture2D(unsigned char* data, const int width_init, const int height_init, const int nrChannels_init, const std::string filename_init)
{
    glGenTextures(1, &id);
    width = width_init;
    height = height_init;
    filename = filename_init;

    format = CalculateFormat(nrChannels_init);


    UpdateTexture();
    // assign texture data
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::~Texture2D()
{
    if (IsLoaded())
    {
        glDeleteTextures(1, &id);
    }
}

const bool Texture2D::IsLoaded() const
{
    return id != -1;
}

const GLuint Texture2D::GetID() const
{
    return id;
}

const int Texture2D::GetWidth() const
{
    return width;
}

const int Texture2D::GetHeight() const
{
    return height;
}

const std::string Texture2D::GetFilename() const
{
    return filename;
}

void Texture2D::SetWrapMode(TEX_WrapMode s, TEX_WrapMode t)
{
    wrapMode[0] = s;
    wrapMode[1] = t;
}

void Texture2D::SetMipMapFilter(TEX_MipMapFiltering filter)
{
    minFilter = filter;
}

void Texture2D::SetFilter(TEX_Filtering filter)
{
    magFilter = filter;
}

void Texture2D::UpdateTexture()
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

TEX_Format Texture2D::CalculateFormat(const int numChannels)
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
