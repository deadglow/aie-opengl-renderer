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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// enable msaa
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	InitUBOs();

	// textures setup
	TextureLoader::Initialise();
	TextureLoader::LoadTexture(DEFAULT_TEXTURE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(DEFAULT_TEXTURE)->GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(DEFAULT_TEXTURE)->GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(DEFAULT_TEXTURE)->GetID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureLoader::GetTexture(DEFAULT_TEXTURE)->GetID());


	// shader setup
	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}
	// set up default material
	Material* material = new Material(ShaderLoader::GetShader(DEFAULT_SHADER), DEFAULT_SHADER);
	material->AddTexture(TextureLoader::GetTexture(DEFAULT_TEXTURE));
	materials.emplace(material->GetName(), material);

	// load models
	ModelLoader::Initialise();
	Model* model = ModelLoader::LoadModel(DEFAULT_MODEL);
	model->SetAllMaterials(materials[DEFAULT_SHADER]);

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

void Renderer::DrawSkybox()
{
	materials.at(SKYBOX_SHADER)->UseShader();

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
	// move da camera
	cameraStack.push_back(Camera());
	cameraStack.front().transform.SetPosition(glm::vec3(0, 0, 5));

	// crate
	Material* material = new Material(ShaderLoader::GetShader("lit"), "crate");
	TextureLoader::LoadTexture("BOX_full_albedo.png");
	TextureLoader::LoadTexture("BOX_full_normal.png");
	material->AddTexture(TextureLoader::GetTexture("BOX_full_albedo.png"));
	material->AddTexture(TextureLoader::GetTexture("BOX_full_normal.png"));
	material->AddProperty("_Smoothness", 1.0f);
	materials.emplace(material->GetName(), material);

	// crate model
	Model* model = ModelLoader::LoadModel("Box_final.obj");
	model->SetMaterial(0, materials["crate"]);

	ModelInstance* modelT = new ModelInstance(model);
	modelInstances.push_back(modelT);

	// beans
	material = new Material(ShaderLoader::GetShader("lit"), "beans");
	TextureLoader::LoadTexture("beancan.png");
	material->AddTexture(TextureLoader::GetTexture("beancan.png"));
	material->AddTexture(TextureLoader::GetTexture(DEFAULT_TEXTURE));
	materials.emplace(material->GetName(), material);
	
	// bean model
	model = ModelLoader::LoadModel("beancan.fbx");
	model->SetMaterial(0, materials["beans"]);


	// create lights
	DirectionalLight* dirLight = new DirectionalLight(glm::normalize(glm::vec3(1, -1, -1)), glm::vec3(1), 1.0f);
	lights.push_back(dirLight);

	PointLight* light = new PointLight();
	lights.push_back(light);

	light->color = { 0.0f, 0.0f, 1.0f };
	light->intensity = 1.0f;
}

void Renderer::Render()
{
	// clear the screen and start drawing
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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