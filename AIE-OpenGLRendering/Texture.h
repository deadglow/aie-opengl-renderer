#pragma once
#include "glad.h"
#include <string>

enum class TEX_Filtering : GLenum
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
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
	INVALID = UINT_MAX
};

enum class TEX_Type : int
{
	Texture1D = GL_TEXTURE_1D,
	Texture2D = GL_TEXTURE_2D,
	Texture3D = GL_TEXTURE_3D,
	Cubemap = GL_TEXTURE_CUBE_MAP,
	Texture1DArray = GL_TEXTURE_1D_ARRAY,
	Texture2DArray = GL_TEXTURE_2D_ARRAY,
	CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY
};

class Texture
{
protected:
	GLuint id = -1;
	std::string filename = "";
	bool renderTexture = false;

	TEX_Filtering minFilter = TEX_Filtering::LinearMipMapLinear;
	TEX_Filtering magFilter = TEX_Filtering::Linear;

public:
	Texture(const std::string filename_init);
	virtual ~Texture();

	const bool IsLoaded() const;

	virtual void Load();
	virtual void Unload();

	void SetID(GLuint newid);
	const GLuint GetID() const;

	const std::string GetFilename() const;

	void SetMinFilter(TEX_Filtering filter);
	void SetMagFilter(TEX_Filtering filter);
	virtual void UpdateTextureProperties();
	virtual TEX_Type GetTexType();


	static TEX_Format CalculateFormat(const int numChannels);
};

