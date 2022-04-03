#include "RenderTexture.h"

RenderTexture::RenderTexture(const std::string filename_init, const int width_init, const int height_init) : Texture2D(filename_init)
{
	width = width_init;
	height = height_init;
}

RenderTexture::~RenderTexture()
{
	Unload();
}

void RenderTexture::Load()
{
	return;
}
