#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MaterialLoader.h"
#include "ModelLoader.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "Lights.h"
#include "MeshDrawData.h"

#define RES_X 1280
#define RES_Y 768
#define WINDOW_NAME "Creamy Triangle Maker"
#define MAX_LIGHTS 5
#define CUBEMAP_TEXTURE_BINDING_START GL_TEXTURE15

#define SHADER_DEFAULT_UNLIT "unlit"
#define SHADER_DEFAULT_LIT "lit"
#define MATERIAL_DEFAULT SHADER_DEFAULT_LIT

#define TEXTURE_DEFAULT_WHITE "_default_white.png"
#define TEXTURE_DEFAULT_ERROR "_default_error.png"
#define TEXTURE_DEFAULT_NORMAL "_default_normal.png"

#define MODEL_DEFAULT "default.obj"

#define SKYBOX_DEFAULT_CUBEMAP "skybox_grass.cubemap"
#define SKYBOX_DEFAULT_SHADER "skybox"
#define SKYBOX_DEFAULT_MODEL "skybox.obj"

#define POSTPROCESS_SHADER "postprocess"

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
	
	static Material* skyboxMaterial;
	static Model* skybox;
	static Mesh* screenPlane;

	static unsigned int renderFramebuffer;
	static unsigned int renderTexture;

	Renderer() = delete;
	~Renderer() = default;

	static void InitUBOs();
	static void SetGlobalsUBO();
	static void SetLightingUBO();
	static void SetFogUBO();

	static void CreateRenderTexture();

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

	// lists
	static std::list<ModelInstance*> modelInstances;
	static std::unordered_map<Material*, std::vector<MeshDrawData>> drawCalls;

	static bool drawDebug;

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

	static void SetSkybox(Cubemap* cubemap);
};