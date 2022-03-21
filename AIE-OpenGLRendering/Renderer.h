#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"
#include "ModelTransform.h"
#include "Camera.h"

#define DEFAULT_SHADER "unlit"
#define RES_X 1920
#define RES_Y 1080
#define WINDOW_NAME "Creamy Triangle Maker"

class Renderer
{
private:
	static GLFWwindow* window;
	static float aspect;
	static double lastTime;
	static double deltaTime;

	Renderer() = delete;
	~Renderer() = default;
	static void OnDraw();
public:
	static Camera camera;
	static std::vector<Model*> modelList;
	static std::vector<ModelTransform*> modelTransforms;
	static std::unordered_map<std::string, ShaderConfiguration*> shaderConfigs;


	static int Initialise();
	static void Shutdown();
	static GLFWwindow* GetWindow();
	static void Start();
	static void Render();

	static double GetDeltaTime();
	static float GetAspect();
	static void ApplyBaseShaderProperties();
};