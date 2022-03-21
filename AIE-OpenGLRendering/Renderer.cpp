#include "Renderer.h"
#include <iostream>
#include "ShaderConfiguration.h"
#include "Vertex.h"
#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"
#include "gtx/norm.hpp"

#define TEXTUREMAN "boletus.png"

//#define TEXTUREMAN "container.jpg"

GLFWwindow* Renderer::window = nullptr;
float Renderer::aspect = 1.0f;
Camera Renderer::camera;
double Renderer::lastTime = 0.0;
double Renderer::deltaTime = 1.0;

std::vector<Model*> Renderer::modelList;
std::vector<ModelTransform*> Renderer::modelTransforms;
std::unordered_map<std::string, ShaderConfiguration*> Renderer::shaderConfigs;


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

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// enable msaa
	glfwWindowHint(GLFW_SAMPLES, 8);
	glEnable(GL_MULTISAMPLE);

	// shader setup
	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}

	// mesh setup
	MeshLoader::Initialise();

	// textures setup
	TextureLoader::Initialise();

	return 0;
}

void Renderer::ApplyBaseShaderProperties()
{
	ShaderLoader::GetCurrentShader()->SetUniform("_Time", (float)glfwGetTime());
}

void Renderer::Shutdown()
{
	TextureLoader::Shutdown();
	MeshLoader::Shutdown();
	ShaderLoader::Shutdown();
	
	for (const auto [key, value] : shaderConfigs)
	{
		delete value;
	}
	for (ModelTransform* mt : modelTransforms)
	{
		delete mt;
	}
	for (Model* model : modelList)
	{
		delete model;
	}

	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return Renderer::window;
}

void Renderer::Start()
{
	// move da camera
	camera.transform = glm::translate(camera.transform, glm::vec3(0, 0, 5));

	// load da texture
	TextureLoader::LoadTexture(TEXTUREMAN);
	ShaderConfiguration* config = new ShaderConfiguration(ShaderLoader::GetShader(DEFAULT_SHADER));
	config->AddProperty<Texture*>("_Texture", TextureLoader::GetTexture(TEXTUREMAN));
	shaderConfigs.emplace(DEFAULT_SHADER, config);

	// create mesh
	glm::vec3 normal = { 0, 0, 1 };
	unsigned char color[4] = {255, 255, 255, 255};

	std::vector<Vertex> vertices;
	vertices.resize(8);
	vertices[0] = Vertex({ -0.5f, -0.5f, 0.5f }, normal, { 0.0f, 1.0f }, color);
	vertices[1] = Vertex({ -0.5f, 0.5f, 0.5f }, normal, { 0.0f, 0.0f }, color);
	vertices[2] = Vertex({ 0.5f, 0.5f, 0.5f }, normal, { 1.0f, 0.0f }, color);
	vertices[3] = Vertex({ 0.5f, -0.5f, 0.5f }, normal, { 1.0f, 1.0f }, color);
	vertices[4] = Vertex({ -0.5f, -0.5f, -0.5f }, normal, { 1.0f, 1.0f }, color);
	vertices[5] = Vertex({ -0.5f, 0.5f, -0.5f }, normal, { 1.0f, 0.0f }, color);
	vertices[6] = Vertex({ 0.5f, 0.5f, -0.5f }, normal, { 0.0f, 0.0f }, color);
	vertices[7] = Vertex({ 0.5f, -0.5f, -0.5f }, normal, { 0.0f, 1.0f }, color);


	std::vector<Triangle> triangles;
	triangles.resize(12);
	Triangle triArray[12]
	{
		{ 0u, 1u, 2u },
		{ 2u, 3u, 0u },
		{ 1u, 5u, 6u },
		{ 6u, 2u, 1u },
		{ 4u, 5u, 6u },
		{ 6u, 7u, 4u },
		{ 0u, 4u, 7u },
		{ 7u, 3u, 0u },
		{ 3u, 2u, 6u },
		{ 6u, 7u, 3u },
		{ 4u, 5u, 1u },
		{ 1u, 0u, 4u }
	};
	memcpy_s(&triangles[0], sizeof(triArray), triArray, sizeof(triArray));

	Mesh* box = new Mesh("Box", vertices, triangles);

	// create model
	Model* model = new Model("box");
	model->AddMesh(box);
	model->AddShaderConfig(shaderConfigs["unlit"]);
	model->SetShaderOfMesh(0, 0);
	modelList.push_back(model);

	model->Load();

	// create model transform
	for (int i = 0; i < 85; ++i)
	{
		ModelTransform* modelT = new ModelTransform(model);
		modelTransforms.push_back(modelT);
		modelT->transform = glm::translate(modelT->transform, glm::vec3(2.0f * i, 0, 0));
	}
}

double Renderer::GetDeltaTime()
{
	return deltaTime;
}

float Renderer::GetAspect()
{
	return aspect;
}

void Renderer::Render()
{
	// clear the screen and start drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calculate delta time
	deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	// drawing here
	OnDraw();

	// swapping buffers
	glfwSwapBuffers(window);

}

void Renderer::OnDraw()
{
	for (int i = 0; i < modelTransforms.size(); ++i)
	{
		modelTransforms[i]->transform = glm::rotate(modelTransforms[i]->transform, glm::radians(10.0f * i) * (float)deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	camera.Draw();
}