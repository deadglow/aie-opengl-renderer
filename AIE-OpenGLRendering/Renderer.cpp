#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "RendererDebugMenu.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "MeshPrimitives.h"
#include <iostream>


GLFWwindow* Renderer::window = nullptr;
float Renderer::aspect = 1.0f;
double Renderer::lastTime = 0.0;
double Renderer::deltaTime = 1.0;

GLuint Renderer::uboCamera = -1;
GLuint Renderer::uboLighting = -1;
GLuint Renderer::uboGlobals = -1;
GLuint Renderer::uboFog = -1;

glm::vec4 Renderer::fogColor = { 0.0f, 0.05f, 0.1f, 1.0f };
float Renderer::fogDensity = 0.000f;

Material* Renderer::skyboxMaterial = nullptr;
Model* Renderer::skybox = nullptr;

std::list<Camera> Renderer::cameraStack;
glm::vec4 Renderer::ambientLight = { 0.1f, 0.1f, 0.1f, 0.0f };
std::vector<Light*> Renderer::lights;

std::list<ModelInstance*> Renderer::modelInstances;
std::unordered_map<std::string, Material*> Renderer::materials;
std::unordered_map<Material*, std::vector<MeshDrawData>> Renderer::drawCalls;


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

	// set clear color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// turn off vsync
	glfwSwapInterval(0);

	// enable msaa
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	// create and initialise UBOs
	InitUBOs();

	// textures setup
	TextureLoader::Initialise();
	TextureLoader::LoadTexture(TEXTURE_DEFAULT_WHITE);
	TextureLoader::LoadTexture(TEXTURE_DEFAULT_ERROR);
	TextureLoader::LoadTexture(TEXTURE_DEFAULT_NORMAL);
	Cubemap* cubemap = TextureLoader::LoadCubemap(SKYBOX_DEFAULT_CUBEMAP);
	for (int i = 0; i < 11; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(TEXTURE_DEFAULT_ERROR)->GetID());
	}

	// shader setup
	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}

	// set skybox texture

	// set up default materials
		// unlit
	Material* material = new Material(ShaderLoader::GetShader(SHADER_DEFAULT_UNLIT), SHADER_DEFAULT_UNLIT);
	material->AddTexture(TextureLoader::GetTexture(TEXTURE_DEFAULT_ERROR));
	materials.emplace(material->GetName(), material);
		// lit
	material = new Material(ShaderLoader::GetShader(SHADER_DEFAULT_LIT), SHADER_DEFAULT_LIT);
	material->AddTexture(TextureLoader::GetTexture(TEXTURE_DEFAULT_WHITE));
	material->AddTexture(TextureLoader::GetTexture(TEXTURE_DEFAULT_NORMAL));
	material->AddTexture(TextureLoader::GetTexture(TEXTURE_DEFAULT_WHITE));
	material->AddTexture(TextureLoader::GetCubemap(SKYBOX_DEFAULT_CUBEMAP));
	material->AddProperty("_Smoothness", 0.3f);
	material->AddProperty("_Reflectivity", 0.3f);
	materials.emplace(material->GetName(), material);
		// skybox
	skyboxMaterial = new Material(ShaderLoader::GetShader(SKYBOX_DEFAULT_SHADER), SKYBOX_DEFAULT_SHADER);
	skyboxMaterial->AddTexture(cubemap);

	// model setup
	ModelLoader::Initialise();
		// default cube
	Model* model = ModelLoader::LoadModel(MODEL_DEFAULT);
	model->SetAllMaterials(materials[SHADER_DEFAULT_UNLIT]);
		// skybox
	skybox = ModelLoader::LoadModel(SKYBOX_DEFAULT_MODEL);
	skybox->SetAllMaterials(skyboxMaterial);

	// set up imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return 0;
}


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
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 2, NULL, GL_STATIC_DRAW);
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

void Renderer::Shutdown()
{
	// shutdown imgui
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	delete skyboxMaterial;

	// shutdown loaders
	ModelLoader::Shutdown();
	TextureLoader::Shutdown();
	ShaderLoader::Shutdown();
	
	// clear materials and instances
	for (const auto [key, value] : materials)
	{
		delete value;
	}
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

void Renderer::SetGlobalsUBO()
{
	float floats[2];
	floats[0] = (float)glfwGetTime();
	floats[1] = (float)deltaTime;
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

	for (int i = 0; i < size; ++i)
	{
		shaderDataArray[i] = lights[i]->ConstructShaderData();
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
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(float), &fogDensity);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

void Renderer::AddModelInstance(ModelInstance* instance)
{
	modelInstances.push_back(instance);
}

void Renderer::RemoveModelInstance(ModelInstance* instance)
{
	modelInstances.remove(instance);
	delete instance;
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	skyboxMaterial->UseShader();
	skybox->meshes[0]->Draw();
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

void Renderer::PrepareDrawCalls()
{
	drawCalls.clear();
	for (const auto value : modelInstances)
	{
		value->AddToDrawCalls();
	}
}

void Renderer::AddToDrawCall(Material* mat, MeshDrawData data)
{
	if (!drawCalls.count(mat))
	{
		drawCalls.emplace(mat, std::vector<MeshDrawData>());
	}
	drawCalls[mat].push_back(data);
}

void Renderer::Start()
{
	cameraStack.push_back(Camera());

	// crate
	Material* material = new Material(ShaderLoader::GetShader(SHADER_DEFAULT_LIT), "crate");
	material->AddTexture(TextureLoader::LoadTexture("BOX_full_albedo.png"));
	material->AddTexture(TextureLoader::LoadTexture("BOX_full_normal.png"));
	material->AddTexture(TextureLoader::GetTexture(TEXTURE_DEFAULT_WHITE));
	material->AddTexture(TextureLoader::GetCubemap(SKYBOX_DEFAULT_CUBEMAP));
	material->AddProperty("_Smoothness", 0.95f);
	material->AddProperty("_Reflectivity", 0.35f);
	materials.emplace(material->GetName(), material);

		// crate model
	Model* model = ModelLoader::LoadModel("Box_final.obj");
	model->SetMaterial(0, materials["crate"]);
	ModelInstance* modelT = new ModelInstance(model);
	modelInstances.push_back(modelT);


	// create lights
	DirectionalLight* dirLight = new DirectionalLight(glm::normalize(glm::vec3(1, -1, -1)), glm::vec3(1), 1.0f);
	lights.push_back(dirLight);

	PointLight* light = new PointLight();
	lights.push_back(light);

	light->color = { 0.0f, 0.0f, 1.0f };
	light->intensity = 1.0f;

	// move da camera
	cameraStack.front().transform.SetPosition(glm::vec3(0, 0, 5));
}


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
	// skybox

	// imgui draw
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// swapping buffers
	glfwSwapBuffers(window);
}

void Renderer::OnDraw()
{
	// set UBOS
	SetGlobalsUBO();
	SetLightingUBO();
	SetFogUBO();

	// prep draw calls
	PrepareDrawCalls();

	auto iter = cameraStack.begin();
	for (iter; iter != cameraStack.end(); iter++)
	{
		(*iter).Draw();
	}
	RendererDebugMenu::DrawImGui();
}