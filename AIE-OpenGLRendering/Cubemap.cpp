#include "Cubemap.h"

Cubemap::Cubemap()
{
}

Cubemap::~Cubemap()
{
}

const bool Cubemap::IsLoaded() const
{
    return false;
}

const GLuint Cubemap::GetID() const
{
    return GLuint();
}

const std::string Cubemap::GetFilename() const
{
    return std::string();
}

void Cubemap::SetWrapMode(TEX_WrapMode s, TEX_WrapMode t)
{
}

void Cubemap::SetMipMapFilter(TEX_MipMapFiltering filter)
{
}

void Cubemap::SetFilter(TEX_Filtering filter)
{
}

void Cubemap::UpdateTexture()
{
}
