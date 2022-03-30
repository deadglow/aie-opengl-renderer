#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "Lights.h"
#include "MeshDrawData.h"

#define RES_X 1920
#define RES_Y 1080
#define WINDOW_NAME "Creamy Triangle Maker"
#define MAX_LIGHTS 5

#define SHADER_DEFAULT_UNLIT "unlit"
#define SHADER_DEFAULT_LIT "lit"

#define TEXTURE_DEFAULT_WHITE "_default_white.png"
#define TEXTURE_DEFAULT_ERROR "_default_error.png"
#define TEXTURE_DEFAULT_NORMAL "_default_normal.png"

#define MODEL_DEFAULT "default.obj"

#define SKYBOX_DEFAULT_CUBEMAP "skybox_grass.cubemap"
#define SKYBOX_DEFAULT_SHADER "skybox"
#define SKYBOX_DEFAULT_MODEL "skybox.obj"

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
	

	Renderer() = delete;
	~Renderer() = default;

	static void InitUBOs();
	static void SetGlobalsUBO();
	static void SetLightingUBO();
	static void SetFogUBO();

	static void OnDraw();
	static void PrepareDrawCalls();
public:
	static std::list<Camera> cameraStack;
	
	// lighting
	static glm::vec4 ambientLight;
	static std::vector<Light*> lights;
	
	// fog
	static glm::vec4 fogColor;
	static float fogDensity;

	// skybox
	static Material* skyboxMaterial;
	static Model* skybox;

	// lists
	static std::list<ModelInstance*> modelInstances;
	static std::unordered_map<std::string, Material*> materials;
	static std::unordered_map<Material*, std::vector<MeshDrawData>> drawCalls;

	static int Initialise();
	static void Shutdown();

	static void Start();
	static void Render();
	static void SetCameraUBO(CameraShaderData csd);
	
	static void AddToDrawCall(Material* mat, MeshDrawData data);
	static void DrawSkybox();
	
	static GLFWwindow* GetWindow();
	static double GetDeltaTime();
	static float GetAspect();

	static void AddModelInstance(ModelInstance* instance);
	static void RemoveModelInstance(ModelInstance* instance);
};