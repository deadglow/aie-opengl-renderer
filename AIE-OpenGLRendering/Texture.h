#pragma once
#include "glad.h"
#include <string>

enum class TEX_Filtering : GLenum
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};

enum class TEX_MipMapFiltering : GLenum
{
	NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
	NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum class TEX_WrapMode : GLenum
{
	Wrap = GL_REPEAT,
	WrapMirror = GL_MIRRORED_REPEAT,
	ClampBorder = GL_CLAMP_TO_BORDER,
	ClampEdge = GL_CLAMP_TO_EDGE,
	ClampMirror = GL_MIRROR_CLAMP_TO_EDGE
};

enum class TEX_Format : GLenum
{
	R = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
};

class Texture
{
private:
	GLuint id = -1;
	int width = -1;
	int height = -1;
	float borderColor[4] = { 1, 1, 1, 1 };
	TEX_Format format = TEX_Format::RGB;
	TEX_WrapMode wrapMode[2] = { TEX_WrapMode::Wrap, TEX_WrapMode::Wrap};
	TEX_MipMapFiltering minFilter = TEX_MipMapFiltering::LinearMipMapLinear;
	TEX_Filtering magFilter = TEX_Filtering::Linear;
	std::string filename = "";
public:
	Texture(unsigned char* data, const int width_init, const int height_init, const int nrChannels_init, const std::string filename_init);
	~Texture();

	const bool IsLoaded() const;
	const GLuint GetID() const;
	const int GetWidth() const;
	const int GetHeight() const;
	const std::string GetFilename() const;

	void SetWrapMode(TEX_WrapMode s, TEX_WrapMode t);
	void SetMipMapFilter(TEX_MipMapFiltering filter);
	void SetFilter(TEX_Filtering filter);
	void UpdateTexture();

};

