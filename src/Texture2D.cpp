#include "Texture2D.h"

Texture2D::Texture2D(const std::string filename_init) : Texture(filename_init)
{
}

Texture2D::~Texture2D()
{
    
}

const int Texture2D::GetWidth() const
{
    return width;
}

const int Texture2D::GetHeight() const
{
    return height;
}

void Texture2D::SetWrapMode(TEX_WrapMode s, TEX_WrapMode t)
{
    wrapMode[0] = s;
    wrapMode[1] = t;
}

void Texture2D::SetProperties(const int width_init, const int height_init, const TEX_Format format_init)
{
    width = width_init;
    height = height_init;
    format = format_init;
    UpdateTextureProperties();
}

void Texture2D::UpdateTextureProperties()
{
    Texture::UpdateTextureProperties();
    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapMode[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapMode[1]);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

TEX_Type Texture2D::GetTexType()
{
    return TEX_Type::Texture2D;
}
