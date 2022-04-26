#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "RendererDebugMenu.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "MeshPrimitives.h"
#include "PostProcessing.h"
#include <iostream>


// window
GLFWwindow* Renderer::window = nullptr;
float Renderer::aspect = 1.0f;

// time
double Renderer::lastTime = 0.0;
double Renderer::deltaTime = 1.0;

// ubos
GLuint Renderer::uboCamera = -1;
GLuint Renderer::uboLighting = -1;
GLuint Renderer::uboGlobals = -1;
GLuint Renderer::uboFog = -1;

// skybox
Material* Renderer::skyboxMaterial = nullptr;
Model* Renderer::skyboxModel = nullptr;
unsigned int Renderer::diffuseIrradianceMap = -1;
unsigned int Renderer::specularIrradianceMap = -1;

// render targets
Mesh* Renderer::screenPlane = nullptr;
RenderTarget* Renderer::mainRenderTarget = nullptr;
RenderTarget *Renderer::brightColorTarget = nullptr;
RenderTarget* Renderer::postprocessingBuffers[2] = { nullptr, nullptr };
bool Renderer::currentPostProcessBuffer = false;
int Renderer::bloomBlurSamples = 5;
float Renderer::bloomThreshold = 1;
float Renderer::exposure = 1.0f;

// fog
glm::vec4 Renderer::fogColor = { 1.0f, 1.0f, 1.0f, 1.0f};
float Renderer::fogDensity = 0.000f;

// stacks
std::list<Camera> Renderer::cameraStack;

// lights
glm::vec4 Renderer::ambientLight = { 1.0f, 1.0f, 1.0f, 1.0f };
std::list<Light*> Renderer::lights;

// drawing and instances
std::list<ModelInstance*> Renderer::modelInstances;
std::unordered_map<Material*, std::vector<MeshDrawData>> Renderer::drawCalls;

// debug
bool Renderer::drawDebug = true;


//==============================================================================
//				FUNCTIONS
//____________________________________________________________________________/

void Renderer::AddModelInstance(ModelInstance* instance)
{
	modelInstances.push_back(instance);
}

void Renderer::RemoveModelInstance(ModelInstance* instance)
{
	modelInstances.remove(instance);
	delete instance;
}

void Renderer::SetCubemap(Cubemap* cubemap, int offset)
{
	glActiveTexture(CUBEMAP_TEXTURE_BINDING_START - offset);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->GetID());
}

void Renderer::SetSkybox(Cubemap* cubemap)
{
	SetCubemap(cubemap, 0);

	if (diffuseIrradianceMap != -1)
		glDeleteTextures(1, &diffuseIrradianceMap);
	
	if (specularIrradianceMap != -1)
		glDeleteTextures(1, &specularIrradianceMap);

	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	
	// generate irradiance

	//create camera
	CameraShaderData csd;
	csd.pMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] = 
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	// generate cubemap
	glGenTextures(1, &diffuseIrradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseIrradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, SKYBOX_IRRADIANCE_SIZE, SKYBOX_IRRADIANCE_SIZE, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// use diffuse irradiance shader
	ShaderLoader::GetShader(SHADER_GEN_DIFFUSE_IRR)->Use();

	// resize viewport
	glViewport(0, 0, SKYBOX_IRRADIANCE_SIZE, SKYBOX_IRRADIANCE_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	// render da sides
	for (unsigned int i = 0; i < 6; ++i)
	{
		csd.vMatrix = captureViews[i];
		SetCameraUBO(csd);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, diffuseIrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxModel->meshes[0]->Draw();
	}

	// bind textures
	glActiveTexture(CUBEMAP_TEXTURE_BINDING_START - 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseIrradianceMap);

	glActiveTexture(CUBEMAP_TEXTURE_BINDING_START - 2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, specularIrradianceMap);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// delete the framebuffer
	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);

	// reset viewport
	glViewport(0,0, RES_X, RES_Y);
}

float Renderer::GetBloomThreshold()
{
	return bloomThreshold;
}

void Renderer::SetBloomThreshold(const float threshold)
{
	ShaderLoader::UseShader(SHADER_DRAWTOBRIGHT);
	ShaderLoader::GetCurrentShader()->SetUniform("_Threshold", threshold);
	bloomThreshold = threshold;
}

float Renderer::GetExposure()
{
	return exposure;
}

void Renderer::SetExposure(const float value)
{
	ShaderLoader::UseShader(SHADER_BLOOMPOST);
	ShaderLoader::GetCurrentShader()->SetUniform("_Exposure", value);
	exposure = value;
}

void Renderer::PrepareDrawCalls()
{
	drawCalls.clear();
	for (const auto value : modelInstances)
	{
		value->AddToDrawCalls();
	}
}

GLFWwindow* Renderer::GetWindow()
{
	return Renderer::window;
}

double Renderer::GetDeltaTime()
{
	return deltaTime;
}

float Renderer::GetAspect()
{
	return aspect;
}

RenderTarget* Renderer::GetMainRenderTarget()
{
	return mainRenderTarget;
}

// UBOS
void Renderer::InitUBOs()
{
	// camera buffer
	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraShaderData), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboCamera);

	// globals buffer
	glGenBuffers(1, &uboGlobals);
	glBindBuffer(GL_UNIFORM_BUFFER, uboGlobals);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 3, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboGlobals);

	// lighting buffer
	glGenBuffers(1, &uboLighting);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(LightShaderData) * MAX_LIGHTS + sizeof(int), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLighting);

	// fog buffer
	glGenBuffers(1, &uboFog);
	glBindBuffer(GL_UNIFORM_BUFFER, uboFog);
	glBufferData(GL_UNIFORM_BUFFER, 20, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboFog);
}

void Renderer::SetGlobalsUBO()
{
	float floats[3];
	floats[0] = (float)glfwGetTime();
	floats[1] = (float)deltaTime;
	floats[2] = (float)aspect;
	glBindBuffer(GL_UNIFORM_BUFFER, uboGlobals);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(floats), floats);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetCameraUBO(CameraShaderData csd)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraShaderData), &csd);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetLightingUBO()
{
	// generate light array
	int size = (int)lights.size();
	LightShaderData shaderDataArray[MAX_LIGHTS];

	int index = 0;
	for (Light* light : lights)
	{
		shaderDataArray[index] = light->ConstructShaderData();
		index++;
	}

	// put the data in
	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &ambientLight);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(LightShaderData) * MAX_LIGHTS, shaderDataArray);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(LightShaderData) * MAX_LIGHTS + sizeof(glm::vec4), sizeof(int), &size);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetFogUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboFog);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &fogColor);
	glm::vec4 fogDensityv4 = {fogDensity, 0, 0, 0};
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &fogDensityv4);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// Render textures
void Renderer::CreateRenderTextures()
{
	mainRenderTarget = new RenderTarget(RES_X, RES_Y, "main.render", GL_FLOAT, true, 1);
	brightColorTarget = new RenderTarget(RES_X, RES_Y, "bright.render", GL_FLOAT, false, 1);
	postprocessingBuffers[0] = new RenderTarget(RES_X, RES_Y, "postprocess0.render", GL_FLOAT, false, 1);
	postprocessingBuffers[1] = new RenderTarget(RES_X, RES_Y, "postprocess1.render", GL_FLOAT, false, 1);
}

void Renderer::RenderPostProcess(Shader* postProcessShader)
{
	// swap to the other buffer
	currentPostProcessBuffer = !currentPostProcessBuffer;

	int currentIndex = (int)currentPostProcessBuffer;
	int otherIndex = (int)!currentPostProcessBuffer;

	// activate da buffer
	postprocessingBuffers[currentIndex]->Use();

	postProcessShader->Use();
	// bind previous buffer or the main buffer
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, postprocessingBuffers[otherIndex]->GetTexture(0)->GetID());

	// draw the plane
	screenPlane->Draw();
}

// Drawing and draw calls
void Renderer::AddToDrawCall(Material* mat, MeshDrawData data)
{
	if (!drawCalls.count(mat))
	{
		drawCalls.emplace(mat, std::vector<MeshDrawData>());
	}
	drawCalls[mat].push_back(data);
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	skyboxMaterial->UseMaterial();
	skyboxModel->meshes[0]->Draw();
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

// Initialisation
int Renderer::Initialise()
{
	// init GLFW, and check if it has successfully inited
	if (!glfwInit())
		return -1;

	// set res and window title here
	window = glfwCreateWindow(RES_X, RES_Y, WINDOW_NAME, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Window failed to create.";
		return -1;
	}
	aspect = (float)RES_X / (float)RES_Y;

	glfwMakeContextCurrent(window);

	// load all glad OpenGL functions
	if (!gladLoadGL())
		return -1;

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// turn off vsync
	glfwSwapInterval(0);

	// enable msaa
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);

	// create and initialise UBOs
	InitUBOs();

	// shader setup
	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}

	TextureLoader::Initialise();
	// default textures setup
	TextureLoader::GetTexture(TEXTURE_DEFAULT_WHITE)->Load();
	TextureLoader::GetTexture(TEXTURE_DEFAULT_BLACK)->Load();
	TextureLoader::GetTexture(TEXTURE_DEFAULT_ERROR)->Load();
	TextureLoader::GetTexture(TEXTURE_DEFAULT_NORMAL)->Load();
	TextureLoader::GetTexture(SKYBOX_DEFAULT_CUBEMAP)->Load();
	TextureLoader::GetTexture(SKYBOX_DEFAULT_IRRADIANCE)->Load();
	for (int i = 0; i < 11; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(TEXTURE_DEFAULT_ERROR)->GetID());
	}

	MaterialLoader::Initialise();
	
	// create skybox
	skyboxMaterial = new Material(ShaderLoader::GetShader(SKYBOX_DEFAULT_SHADER), SKYBOX_DEFAULT_SHADER);

	ModelLoader::Initialise();
	// default cube
	Model* model = ModelLoader::LoadModel(MODEL_DEFAULT);
	model->SetAllMaterials(MaterialLoader::GetMaterial(SHADER_DEFAULT_UNLIT));
	// skybox
	skyboxModel = ModelLoader::LoadModel(SKYBOX_DEFAULT_MODEL);
	skyboxModel->SetAllMaterials(skyboxMaterial);

	// set up screen plane
	screenPlane = MeshPrimitives::CreatePlane(1, 1, { 0, 0, -1 }, { 1, 0, 0 });

	CreateRenderTextures();

	// set skybox and irradiance binding
	SetSkybox(TextureLoader::GetCubemap(SKYBOX_DEFAULT_CUBEMAP));

	PostProcessing::Initialise();

	// set up imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return 0;
}

// Shutdown
void Renderer::Shutdown()
{
	// shutdown imgui
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	delete mainRenderTarget;
	delete brightColorTarget;
	delete postprocessingBuffers[0];
	delete postprocessingBuffers[1];
	delete skyboxMaterial;
	delete screenPlane;

	// shutdown loaders
	PostProcessing::Shutdown();
	ModelLoader::Shutdown();
	MaterialLoader::Shutdown();
	TextureLoader::Shutdown();
	ShaderLoader::Shutdown();

	// clear materials and instances
	for (ModelInstance* mt : modelInstances)
	{
		delete mt;
	}
	for (Light* light : lights)
	{
		delete light;
	}

	glfwTerminate();
}

// Render
void Renderer::Render()
{
	// imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// calculate delta time
	deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	// drawing here
	OnDraw();

	// imgui draw
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// swapping buffers
	glfwSwapBuffers(window);
}

// Non essential init here
void Renderer::Start()
{
	cameraStack.push_back(Camera());

	// create lights
	DirectionalLight* dirLight = new DirectionalLight(glm::normalize(glm::vec3(1, -1, -1)), glm::vec3(1), 1.0f);
	lights.push_back(dirLight);

	Model* sphere = ModelLoader::GetModel("uvsphere.obj");
	sphere->Load();

	// generate sphere thing
	for (int y = 0; y < 6; ++y)
	{
		for (int x = 0; x < 6; ++x)
		{
			ModelInstance* instance = new ModelInstance(sphere);
			glm::vec3 pos = {0, 0, 0};
			pos.x = x * 2.5f;
			pos.y = y * 2.5f;
			instance->transform.SetPosition(pos);
			AddModelInstance(instance);
			Material* mat = MaterialLoader::CloneMaterial(MaterialLoader::GetMaterial(SHADER_DEFAULT_LIT), "sphere " + std::to_string(y * 6 + x));
			mat->AddProperty("_AlbedoColor", glm::vec3{0.0f, 0.0f, 1.0f});
			mat->AddProperty("_MetallicScale", y / 5.0f);
			mat->AddProperty("_RoughnessScale", x / 5.0f);
			instance->SetMaterialOverride(0, mat);
		}
	}

	// move da camera
	cameraStack.front().transform.SetPosition(glm::vec3(0, 0, 5));
}

// Non engine drawing here
void Renderer::OnDraw()
{
	// set UBOS
	SetGlobalsUBO();
	SetLightingUBO();
	SetFogUBO();

	PrepareDrawCalls();

	// draw to render texture
	auto iter = cameraStack.begin();
	for (iter; iter != cameraStack.end(); iter++)
	{
		(*iter).Draw();
	}

	// do post processing
	// bind main buffer to 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mainRenderTarget->GetTexture(0)->GetID());


	// add all bright colours to the bright texture
	brightColorTarget->Use();
	ShaderLoader::UseShader(SHADER_DRAWTOBRIGHT);
	screenPlane->Draw();

	// bind bright texture to 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, brightColorTarget->GetTexture(0)->GetID());

	// bloom. Ping pong between postprocessbuffers and continue to blur based on number of samples
	bool horizontal = true;
	bool firstIteration = true;

	int amount = bloomBlurSamples * 2;
	ShaderLoader::UseShader(SHADER_BLOOM);
	for (int i = 0; i < amount; ++i)
	{
		ShaderLoader::GetCurrentShader()->SetUniform("_UseMain", firstIteration);
		ShaderLoader::GetCurrentShader()->SetUniform("_Horizontal", horizontal);

		RenderPostProcess(ShaderLoader::GetCurrentShader());

		horizontal = !horizontal;
		if (firstIteration)
			firstIteration = false;
	}

	// combine bloom here
	ShaderLoader::UseShader(SHADER_BLOOMPOST);
	RenderPostProcess(ShaderLoader::GetShader(SHADER_BLOOMPOST));

	// render other post processing
	PostProcessingStack* stack = PostProcessing::stacks[POSTPROCESS_STACK];
	for (int i = 0; i < stack->shaderList.size(); ++i)
	{
		RenderPostProcess(ShaderLoader::GetShader(stack->shaderList[i]));
	}

	// draw to default surface
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, postprocessingBuffers[(int)currentPostProcessBuffer]->GetTexture(0)->GetID());

	RenderTarget::UseDefault();
	ShaderLoader::UseShader(SHADER_SCREENRENDER);
	ShaderLoader::GetCurrentShader()->SetUniform("_UseMainBuffer", false);
	screenPlane->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	// imgui
	if (drawDebug)
		RendererDebugMenu::DrawImGui();
}
