#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"

#define DEFAULT_SHADER "default"

class Renderer
{
private:
	static ShaderLoader* shaderLoader;
	static GLFWwindow* window;

	static void OnDraw();
	Renderer() = default;
	~Renderer() = default;
public:

	static int Initialise();
	static void Shutdown();
	static ShaderLoader* GetShaderLoader();
	static GLFWwindow* GetWindow();
	static void Render();
};

