#pragma once
#define GLFW_INCLUDE_NONE
#include "glfw3.h"
#include "glad.h"
#include "glm.hpp"
#include <iostream>
#include "ShaderLoader.h"

#define DEFAULT_SHADER "default"

GLFWwindow* window;
ShaderLoader* shaders;

void OnInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R)
		{
			shaders->InitialiseShaders();
		}
	}
}

void Startup()
{

}

void Draw()
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
	};

	Vertex vertices[4];
	vertices[0] = { -1.0f, -1.0f, 1,
					255, 0, 0, 255 };
	vertices[1] = { -1.0f, 1.0f, 1,
					0, 255, 0, 255 };
	vertices[2] = { 1.0f, 1.0f, 1,
					0, 0, 255, 255 };
	vertices[3] = { 1.0f, -1.0f, 1,
					127, 127, 0, 255 };

	unsigned int indices[6] =
	{
		0, 1, 2,
		0, 2, 3
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vertex::pos)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	/*if (shaders->GetCurrentShader() != nullptr)
		shaders->GetCurrentShader()->SetUniform("_Offset", glm::vec4(glm::sin((float)glfwGetTime()) * 0.2f, 0.0f, 0.0f, 0.0f));*/
	shaders->UseShader("vertcolor");
	shaders->GetCurrentShader()->SetUniform("_Time", (float)glfwGetTime());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

int main()
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

	// startup stuff here
	shaders = new ShaderLoader();
	
	if (!shaders->InitialiseShaders())
	{
		delete shaders;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, OnInput);

	Startup();

	// main game loop
	while (shaders->GetShaderStateOkay() && !glfwWindowShouldClose(window))
	{
		// clear the screen and start drawing
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();

		// swapping buffers
		glfwSwapBuffers(window);

		// poll for input and other events
		glfwPollEvents();
	}

	// window has closed, clean up GLFW and exit
	delete shaders;
	glfwTerminate();

	return 0;
}