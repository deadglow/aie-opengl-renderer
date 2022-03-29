#pragma once
#include "glad.h"
#include "Texture.h"
#include <string>



class Texture2D :
	public Texture
{
private:
	int width = -1;
	int height = -1;
	float borderColor[4] = { 1, 1, 1, 1 };
	TEX_Format format = TEX_Format::RGB;
	TEX_WrapMode wrapMode[2] = { TEX_WrapMode::Wrap, TEX_WrapMode::Wrap};
public:
	Texture2D(unsigned int id_init, const std::string filename_init, const int width_init, const int height_init, TEX_Format format_init);
	~Texture2D();

	const int GetWidth() const;
	const int GetHeight() const;

	void SetWrapMode(TEX_WrapMode s, TEX_WrapMode t);
	void UpdateTextureProperties();
	TEX_Type GetTexType();
};