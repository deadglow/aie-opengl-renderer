#pragma once
#include "glm/glm.hpp"
#include "glad.h"
#include "RenderTexture.h"
#include <string>

class RenderTarget
{
private:
	GLuint framebufferID = -1;
	int colorBuffers = -1;
	RenderTexture** renderTextures;
	GLuint depthStencilBufferID = -1;

	std::string name;

	float width = -1;
	float height = -1;
public:
	RenderTarget(const int width_init, const int height_init, const std::string name_init, const GLenum precisionType, const bool useDepth = true, const int colorBufferCount = 1);
	~RenderTarget();

	void Use();
	void Clear();
	const bool UsesDepth() const;

	Texture2D* GetTexture(const int index) const;
	const int GetTextureCount() const;

	static void UseDefault();
};

