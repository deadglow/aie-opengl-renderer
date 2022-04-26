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
#include "RenderTarget.h"

#define RES_X 1280
#define RES_Y 768
#define WINDOW_NAME "Creamy Triangle Maker"
#define MAX_LIGHTS 5
#define CUBEMAP_TEXTURE_BINDING_START GL_TEXTURE15

#define SHADER_GEN_IRR_DIFFUSE "irradianceDiff"
#define SHADER_GEN_IRR_SPEC "irradianceSpec"
#define SHADER_DEFAULT_UNLIT "unlit"
#define SHADER_DEFAULT_LIT "pbr"
#define MATERIAL_DEFAULT SHADER_DEFAULT_LIT

#define TEXTURE_DEFAULT_WHITE "_default_white.png"
#define TEXTURE_DEFAULT_BLACK "_default_black.png"
#define TEXTURE_DEFAULT_ERROR "_default_error.png"
#define TEXTURE_DEFAULT_NORMAL "_default_normal.png"

#define MODEL_DEFAULT "default.obj"

#define SKYBOX_DEFAULT_CUBEMAP "skybox_island.cubemap"
#define SKYBOX_DEFAULT_SHADER "skybox"
#define SKYBOX_DEFAULT_MODEL "skybox.obj"
#define SKYBOX_IRRADIANCE_SIZE 32

#define SHADER_SCREENRENDER "post_screenrender"
#define SHADER_BLOOM "post_bloom"
#define SHADER_BLOOMPOST "post_bloomPost"
#define SHADER_DRAWTOBRIGHT "post_drawtobright"

#define POSTPROCESS_STACK "post"

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
	static Model* skyboxModel;
	static unsigned int diffuseIrradianceMap;
	static unsigned int specularIrradianceMap;

	static Mesh* screenPlane;

	static RenderTarget* mainRenderTarget;
	static RenderTarget* brightColorTarget;
	static RenderTarget* postprocessingBuffers[2];
	static bool currentPostProcessBuffer;
	static float bloomThreshold;
	static float exposure;

	Renderer() = delete;
	~Renderer() = default;

	static void InitUBOs();
	static void SetGlobalsUBO();
	static void SetLightingUBO();
	static void SetFogUBO();

	static void CreateRenderTextures();

	static void RenderPostProcess(Shader* postProcessShader);

	static void OnDraw();
	static void PrepareDrawCalls();
public:
	static std::list<Camera> cameraStack;
	
	// lighting
	static glm::vec4 ambientLight;
	static std::list<Light*> lights;
	static int bloomBlurSamples;
	
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

	static RenderTarget* GetMainRenderTarget();

	static void AddModelInstance(ModelInstance* instance);
	static void RemoveModelInstance(ModelInstance* instance);

	static void SetCubemap(Cubemap* cubemap, int offset);
	static void SetSkybox(Cubemap* cubemap);

	static float GetBloomThreshold();
	static void SetBloomThreshold(const float threshold);

	static float GetExposure();
	static void SetExposure(const float value);
};