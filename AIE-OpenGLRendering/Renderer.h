#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"

#define DEFAULT_SHADER "unlit"

class Renderer
{
private:
	static GLFWwindow* window;

	static void OnDraw();
	Renderer() = default;
	~Renderer() = default;
public:

	static int Initialise();
	static void Shutdown();
	static GLFWwindow* GetWindow();
	static void Start();
	static void Render();
};

