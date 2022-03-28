#pragma once
#include "Texture.h"

class Cubemap
{
private:
	GLuint id = -1;
	TEX_Format format = TEX_Format::RGB;
	TEX_MipMapFiltering minFilter = TEX_MipMapFiltering::LinearMipMapLinear;
	TEX_Filtering magFilter = TEX_Filtering::Linear;
	std::string filename = "";
public:
	Cubemap();
	~Cubemap();

	const bool IsLoaded() const;
	const GLuint GetID() const;
	const std::string GetFilename() const;

	void SetWrapMode(TEX_WrapMode s, TEX_WrapMode t);
	void SetMipMapFilter(TEX_MipMapFiltering filter);
	void SetFilter(TEX_Filtering filter);
	void UpdateTexture();

};

