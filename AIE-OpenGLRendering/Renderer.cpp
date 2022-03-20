#include "Renderer.h"
#include <iostream>
#include "ShaderConfiguration.h"
#include "Vertex.h"
#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"
#include "gtx/norm.hpp"

#define TEXTUREMAN "boletus.png"
#define FOV 70.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 200.0f

//#define TEXTUREMAN "container.jpg"

GLFWwindow* Renderer::window = nullptr;
float Renderer::aspect;

std::unordered_map<std::string, ShaderConfiguration*> shaderConfigs;
Mesh* box;


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
	shaderConfigs.clear();

	delete box;

	glfwTerminate();
}

GLFWwindow* Renderer::GetWindow()
{
	return Renderer::window;
}

void Renderer::Start()
{
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

	box = new Mesh("Plane", vertices, triangles);

	box->LoadMesh();
}

void Renderer::Render()
{
	// clear the screen and start drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// drawing here
	OnDraw();

	// swapping buffers
	glfwSwapBuffers(window);
}

void Renderer::OnDraw()
{
	shaderConfigs.at("unlit")->UseShader();

	// generate matrices
	glm::mat4 m2w = glm::mat4(1.0f);
	m2w = glm::rotate(m2w, glm::radians((float)glfwGetTime() * 40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 w2v = glm::mat4(1.0f);
	w2v = glm::translate(w2v, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspect, NEAR_PLANE, FAR_PLANE);
	
	glm::mat4 mvp = projection * w2v * m2w;

	ApplyBaseShaderProperties();
	ShaderLoader::GetCurrentShader()->SetUniform("_MVP", mvp);

	box->Draw();
}