#include "Cubemap.h"


Cubemap::Cubemap(GLuint id_init, const std::string filename_init) : Texture(id_init, filename_init)
{
    id = id_init;
    filename = filename_init;

    UpdateTextureProperties();
}

Cubemap::~Cubemap()
{
    if (IsLoaded())
    {
        glDeleteTextures(1, &id);
    }
}

void Cubemap::UpdateTextureProperties()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLenum)minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLenum)magFilter);

    // unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TEX_Type Cubemap::GetTexType()
{
    return TEX_Type::Cubemap;
}
