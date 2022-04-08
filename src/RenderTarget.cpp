#include "RenderTarget.h"
#include "Texture.h"
#include "TextureLoader.h"
#include <iostream>

RenderTarget::RenderTarget(const int width_init, const int height_init, const std::string name_init, const GLenum precisionType, const bool useDepth, const int colorBufferCount)
{
	width = width_init;
	height = height_init;
	name = name_init;

	// create render texture
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	colorBuffers = colorBufferCount;

	renderTextures = new RenderTexture*[colorBuffers];

	for (int i = 0; i < colorBuffers; ++i)
	{
		// color buffer
		GLuint colorTexID;
		glGenTextures(1, &colorTexID);
		glBindTexture(GL_TEXTURE_2D, colorTexID);
		
		GLint format = (GLint)TEX_Format::RGBA;

		// change the format if its a float format
		if (precisionType == GL_FLOAT)
			format = GL_RGBA16F;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, (GLint)TEX_Format::RGBA, precisionType, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexID, 0);

		RenderTexture* rentex = new RenderTexture(name, width, height);
		rentex->SetID(colorTexID);
		rentex->SetMagFilter(TEX_Filtering::Linear);
		rentex->SetMinFilter(TEX_Filtering::Linear);
		rentex->SetWrapMode(TEX_WrapMode::ClampEdge, TEX_WrapMode::ClampEdge);
		rentex->SetProperties(width, height, TEX_Format::RGB);

		TextureLoader::AddTexture(rentex);

		renderTextures[i] = rentex;
	}

	// depth and stencil buffer
	if (useDepth)
	{
		glGenRenderbuffers(1, &depthStencilBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBufferID);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Render texture not complete: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTarget::~RenderTarget()
{
	delete[] renderTextures;
	TextureLoader::RemoveTexture(name);

	if (depthStencilBufferID != -1)
		glDeleteRenderbuffers(1, &depthStencilBufferID);

	glDeleteFramebuffers(1, &framebufferID);
}

void RenderTarget::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	if (UsesDepth())
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void RenderTarget::Clear()
{
	Use();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

const bool RenderTarget::UsesDepth() const
{
	return depthStencilBufferID != -1;
}

Texture2D* RenderTarget::GetTexture(const int index) const
{
	return (Texture2D*)renderTextures[index];
}

void RenderTarget::UseDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

const int RenderTarget::GetTextureCount() const
{
    return colorBuffers;
}
