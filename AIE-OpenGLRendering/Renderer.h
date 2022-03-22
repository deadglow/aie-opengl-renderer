#pragma once
#include "glad.h"
#include "glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "ModelTransform.h"
#include "Camera.h"
#include "DirectionalLight.h"

#define DEFAULT_SHADER "lit"
#define RES_X 1280
#define RES_Y 720
#define WINDOW_NAME "Creamy Triangle Maker"

class Renderer
{
private:
	static GLFWwindow* window;
	static GLuint uboCamera;
	static GLuint uboLighting;
	static GLuint uboFog;
	static float aspect;
	static double lastTime;
	static double deltaTime;
	static glm::vec4 fogColor;
	static float fogDensity;

	Renderer() = delete;
	~Renderer() = default;
	static void OnDraw();
	static void InitUBOs();
	static void SetLightingUBO();
	static void SetFogUBO();
public:
	static Camera camera;
	static glm::vec4 ambientLight;
	static DirectionalLight dirLight;
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
	static void SetCameraUBO(CameraShaderData csd);
	static void ApplyBaseShaderProperties();
};