#pragma once
#include "Texture2D.h"

class RenderTexture
	: public Texture2D
{
private:
	
public:
	RenderTexture(const std::string filename_init, const int width_init, const int height_init);
	~RenderTexture();

	void Load();
};

