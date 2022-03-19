#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"

#define DEFAULT_SHADER "default"

class Renderer
{
private:
	static GLFWwindow* window;
	static ShaderLoader* shaderLoader;
	static TextureLoader* textureLoader;

	static void OnDraw();
	Renderer() = default;
	~Renderer() = default;
public:

	static int Initialise();
	static void Shutdown();
	static ShaderLoader* GetShaderLoader();
	static TextureLoader* GetTextureLoader();
	static GLFWwindow* GetWindow();
	static void Start();
	static void Render();
};

