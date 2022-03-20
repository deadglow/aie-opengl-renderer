#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"

#define DEFAULT_SHADER "unlit"
#define RES_X 1280
#define RES_Y 720
#define WINDOW_NAME "Creamy Triangle Maker"

class Renderer
{
private:
	static GLFWwindow* window;
	static float aspect;

	Renderer() = delete;
	~Renderer() = default;
	static void OnDraw();
	static void ApplyBaseShaderProperties();
public:

	static int Initialise();
	static void Shutdown();
	static GLFWwindow* GetWindow();
	static void Start();
	static void Render();
};

