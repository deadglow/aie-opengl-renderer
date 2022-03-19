#include "Renderer.h"
#include <iostream>
#include "ShaderConfiguration.h"

#define TEXTUREMAN "boletus.png"
//#define TEXTUREMAN "container.jpg"

GLFWwindow* Renderer::window = nullptr;

std::unordered_map<std::string, ShaderConfiguration*> shaderConfigs;


int Renderer::Initialise()
{
	// init GLFW, and check if it has successfully inited
	if (!glfwInit())
		return -1;

	// set res and window title here
	window = glfwCreateWindow(1280, 720, "Creamy Triangle Maker", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Window failed to create.";
		return -1;
	}

	glfwMakeContextCurrent(window);

	// load all glad OpenGL functions
	if (!gladLoadGL())
		return -1;

	// shader setup

	if (!ShaderLoader::InitialiseShaders())
	{
		ShaderLoader::Shutdown();
		glfwTerminate();
		return -1;
	}

	// textures setup
	TextureLoader::Initialise();
	// textureLoader->PrintAllTextureFiles();

	// mesh setup
	MeshLoader::Initialise();

	return 0;
}

void Renderer::Shutdown()
{
	MeshLoader::Shutdown();
	TextureLoader::Shutdown();
	ShaderLoader::Shutdown();
	for (const auto [key, value] : shaderConfigs)
	{
		delete value;
	}

	shaderConfigs.clear();

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
	shaderConfigs.emplace("unlit", config);
}

void Renderer::Render()
{
	// clear the screen and start drawing
	glClear(GL_COLOR_BUFFER_BIT);

	// drawing here
	OnDraw();

	// swapping buffers
	glfwSwapBuffers(window);
}

void Renderer::OnDraw()
{
	GLuint vao[2];
	GLuint vbo;
	GLuint ebo;
	glGenVertexArrays(1, &vao[0]);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	struct Vertex
	{
		float pos[3];
		unsigned char color[4];
		float texCoords[2];
	};

	Vertex vertices[4];
	float val = -0.5f + glm::sin(glfwGetTime()) * 0.1f;
	vertices[0] =
	{	-0.5f, -0.5f, 1,
		255, 255, 255, 255,
		0.0f, 1.0f
	};
	vertices[1] =
	{	-0.5f, 0.5f, 1,
		255, 255, 255, 255,
		0.0f, 0.0f
	};
	vertices[2] = 
	{	0.5f, 0.5f, 1,
		255, 255, 255, 255,
		1.0f, 0.0f
	};

	vertices[3] =
	{	0.5f, -0.5f, 1,
		255, 255, 255, 255,
		1.0f, 1.0f
	};

	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	// bind the vao
	glBindVertexArray(vao[0]);
	// bind the triangle buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	// bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	// set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vertex::pos)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::pos) + sizeof(Vertex::color)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	shaderConfigs.at("unlit")->UseShader();
	ShaderLoader::GetCurrentShader()->SetUniform("_Time", (float)glfwGetTime());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}