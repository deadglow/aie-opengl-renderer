#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "ModelTransform.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#define RES_X 1280
#define RES_Y 720
#define WINDOW_NAME "Creamy Triangle Maker"
#define MAX_LIGHTS 5

#define DEFAULT_SHADER "lit"
#define DEFAULT_TEX "default.png"

class Renderer
{
private:
	static GLFWwindow* window;
	static float aspect;
	static double lastTime;
	static double deltaTime;
	
	// ubo
	static GLuint uboCamera;
	static GLuint uboGlobals;
	static GLuint uboLighting;
	static GLuint uboFog;
	
	// fog
	static glm::vec4 fogColor;
	static float fogDensity;

	Renderer() = delete;
	~Renderer() = default;
	static void OnDraw();
	static void InitUBOs();
	static void SetGlobalsUBO();
	static void SetLightingUBO();
	static void SetFogUBO();
public:
	static Camera camera;
	
	// lighting
	static glm::vec4 ambientLight;
	static std::vector<Light*> lights;
	
	// lists
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
};