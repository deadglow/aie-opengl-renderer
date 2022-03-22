#include "Renderer.h"
#include <iostream>
#include "ShaderConfiguration.h"
#include "MeshPrimitives.h"
#include "glm.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"

#define TEXTUREMAN "boletus.png"

GLFWwindow* Renderer::window = nullptr;
float Renderer::aspect = 1.0f;
double Renderer::lastTime = 0.0;
double Renderer::deltaTime = 1.0;

GLuint Renderer::uboCamera = -1;
GLuint Renderer::uboLighting = -1;
GLuint Renderer::uboLights = -1;
GLuint Renderer::uboFog = -1;

glm::vec4 Renderer::fogColor = { 0.0f, 0.05f, 0.1f, 1.0f };
float Renderer::fogDensity = 0.000f;

Camera Renderer::camera;
glm::vec4 Renderer::ambientLight = { 0.1f, 0.1f, 0.1f, 0.0f };
DirectionalLight Renderer::dirLight = DirectionalLight();

std::vector<Model*> Renderer::modelList;
std::vector<ModelTransform*> Renderer::modelTransforms;
std::unordered_map<std::string, ShaderConfiguration*> Renderer::shaderConfigs;
std::vector<PointLight> Renderer::lights;


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

	glClearColor(0.0f, 0.02f, 0.07f, 1.0f);

	// enable msaa
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	InitUBOs();

	// shader setup
	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}

	// textures setup
	TextureLoader::Initialise();

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

	// lighting buffer
	glGenBuffers(1, &uboLighting);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLighting);

	// lights buffer
	glGenBuffers(1, &uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3 * MAX_LIGHTS, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboLights);

	// fog buffer
	glGenBuffers(1, &uboFog);
	glBindBuffer(GL_UNIFORM_BUFFER, uboFog);
	glBufferData(GL_UNIFORM_BUFFER, 20, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, 3, uboFog);
}

void Renderer::SetCameraUBO(CameraShaderData csd)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraShaderData), &csd);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetLightingUBO()
{
	// create dir light data
	glm::vec4 dirLightData[3];
	dirLightData[0] = glm::vec4(dirLight.direction, 1.0f);
	dirLightData[1] = dirLight.GetFinalColor();
	dirLightData[2] = ambientLight;

	glBindBuffer(GL_UNIFORM_BUFFER, uboLighting);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4) * 3, dirLightData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetLightsUBO()
{
	constexpr int pointLightSize = sizeof(glm::vec4) * 3;

	// generate light array
	PointLight lightArray[MAX_LIGHTS];

	int size = glm::min((int)lights.size(), MAX_LIGHTS);

	for (int i = 0; i < size; ++i)
	{
		lightArray[i] = lights[i];
	}

	int diff = MAX_LIGHTS - size;

	// fill default lights
	PointLight defaultLight = PointLight();
	defaultLight.intensity = 0;
	for (int i = size; i < MAX_LIGHTS; ++i)
	{
		lightArray[i] = defaultLight;
	}

	// generate data array
	glm::vec4 floats[pointLightSize * MAX_LIGHTS];

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		glm::vec4 position;
		glm::vec4 color;
		glm::vec4 properties;
		position = glm::vec4(lightArray[i].position, 1.0f);
		color = glm::vec4(lightArray[i].GetFinalColor());
		properties.x = lightArray[i].constant;
		properties.y = lightArray[i].linear;
		properties.z = lightArray[i].quadratic;
		properties.w = lightArray[i].radius;

		int shift = i * 3;
		floats[shift + 0] = position;
		floats[shift + 1] = color;
		floats[shift + 2] = properties;
	}

	// put the data in
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, pointLightSize * MAX_LIGHTS, floats);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetFogUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboFog);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &fogColor);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(float), &fogDensity);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::ApplyBaseShaderProperties()
{
	Shader* shader = ShaderLoader::GetCurrentShader();
	shader->SetUniform("_Time", (float)glfwGetTime());
}

void Renderer::Shutdown()
{
	TextureLoader::Shutdown();
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
	
	// create model
	Mesh* box = MeshPrimitives::CreateCube(1.0f, 1.0f, 1.0f);
	Model* model = new Model("box");
	model->AddMesh(box);
	model->AddShaderConfig(shaderConfigs[DEFAULT_SHADER]);
	model->SetShaderOfMesh(0, 0);
	modelList.push_back(model);	

	model->Load();

	// create model transform
	for (int i = 0; i < 85; ++i)
	{
		ModelTransform* modelT = new ModelTransform(model);
		modelTransforms.push_back(modelT);
		modelT->transform = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f * i, 0, 0));
	}

	// create lights
	PointLight light = PointLight();
	light.position = { 5.0f, 0.0f, 2.0f };
	light.color = { 1.0f, 0.0f, 0.0f };
	light.intensity = 2.5f;

	lights.push_back(light);
	lights.push_back(light);
	lights[1].color = { 0.0f, 0.0f, 1.0f };
	lights[1].intensity = 4.0f;
	lights[0].radius = 1.0f;
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
	// set UBOS
	SetLightingUBO();
	SetLightsUBO();
	SetFogUBO();

	for (int i = 0; i < modelTransforms.size(); ++i)
	{
		modelTransforms[i]->transform = glm::rotate(modelTransforms[i]->transform, glm::radians(10.0f * i) * (float)deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	camera.Draw();
}

