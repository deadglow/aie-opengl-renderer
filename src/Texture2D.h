#pragma once
#include "glad.h"
#include "Texture.h"
#include <string>

class Texture2D :
	public Texture
{
protected:
	int width = -1;
	int height = -1;
	float borderColor[4] = { 1, 1, 1, 1 };
	TEX_Format format = TEX_Format::RGB;
	TEX_WrapMode wrapMode[2] = { TEX_WrapMode::Wrap, TEX_WrapMode::Wrap};

public:
	Texture2D(const std::string filename_init);
	~Texture2D();

	const int GetWidth() const;
	const int GetHeight() const;

	void SetWrapMode(TEX_WrapMode s, TEX_WrapMode t);
	void SetProperties(const int width_init, const int height_init, const TEX_Format format_init);
	void UpdateTextureProperties();

	TEX_Type GetTexType();
};