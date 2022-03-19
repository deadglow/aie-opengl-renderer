#include "Texture.h"

Texture::Texture(unsigned char* data, const int width_init, const int height_init, const int nrChannels_init, const std::string filename_init)
{
    glGenTextures(1, &id);
    width = width_init;
    height = height_init;
    nrChannels = nrChannels_init;
    filename = filename_init;

    UpdateTexture();
    // assign texture data
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // texture params
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

void Texture::SetWrapMode(GLint s, GLint t)
{
    wrapMode[0] = s;
    wrapMode[1] = t;
}

void Texture::SetMinFilter(GLint filter)
{
    minFilter = filter;
}

void Texture::SetMagFilter(GLint filter)
{
    magFilter = filter;
}

void Texture::UpdateTexture()
{
    glBindTexture(GL_TEXTURE_2D, id);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
    // unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}
