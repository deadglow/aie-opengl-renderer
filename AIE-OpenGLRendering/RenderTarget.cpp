#include "RenderTarget.h"
#include "Texture.h"
#include "TextureLoader.h"
#include <iostream>

RenderTarget::RenderTarget(const int width_init, const int height_init, const std::string name_init, const bool useDepth)
{
	width = width_init;
	height = height_init;
	name = name_init;

	// create render texture
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	// color buffer
	GLuint colorTexID;
	glGenTextures(1, &colorTexID);
	glBindTexture(GL_TEXTURE_2D, colorTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)TEX_Format::RGB, width, height, 0, (GLint)TEX_Format::RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexID, 0);

	renderTexture = new RenderTexture(name, width, height);
	renderTexture->SetID(colorTexID);
	renderTexture->SetMagFilter(TEX_Filtering::Linear);
	renderTexture->SetMinFilter(TEX_Filtering::Linear);
	renderTexture->SetProperties(width, height, TEX_Format::RGB);

	TextureLoader::AddTexture(renderTexture);

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
		std::cout << "Render texture not complete." << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTarget::~RenderTarget()
{
	delete renderTexture;
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

Texture2D* RenderTarget::GetTexture() const
{
	return (Texture2D*)renderTexture;
}

void RenderTarget::UseDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}
