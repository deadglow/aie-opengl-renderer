#include "Texture2D.h"

Texture2D::Texture2D(unsigned int id_init, const std::string filename_init, const int width_init, const int height_init, TEX_Format format_init) : Texture(id_init, filename_init)
{
    id = id_init;
    width = width_init;
    height = height_init;
    filename = filename_init;
    format = format_init;

    UpdateTextureProperties();
}

Texture2D::~Texture2D()
{
    if (IsLoaded())
    {
        glDeleteTextures(1, &id);
    }
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
