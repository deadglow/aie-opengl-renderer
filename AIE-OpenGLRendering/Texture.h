#pragma once
#include "glad.h"
#include <string>

class Texture
{
private:
	GLuint id = -1;
	int width = -1;
	int height = -1;
	int nrChannels = 0;
	float borderColor[4] = { 1, 1, 1, 1 };
	GLenum wrapMode[2] = { GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER };
	GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
	GLenum magFilter = GL_LINEAR;
	std::string filename = "";
public:
	Texture(unsigned char* data, const int width_init, const int height_init, const int nrChannels_init, const std::string filename_init);
	~Texture();

	const bool IsLoaded() const;
	const GLuint GetID() const;
	const int GetWidth() const;
	const int GetHeight() const;
	void SetWrapMode(GLenum s, GLenum t);
	void SetMinFilter(GLenum filter);
	void SetMagFilter(GLenum filter);
	void UpdateTexture();
};

