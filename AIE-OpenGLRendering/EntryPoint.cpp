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
GLuint triangleID;

void Startup()
{

}

void Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, triangleID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
	shaders = new ShaderLoader("", "shader.list");
	shaders->LoadShaders();
	shaders->UseShader(DEFAULT_SHADER);
	
	Startup();

	glGenBuffers(1, &triangleID);

	float aBunchOfFloats[] = { 0.0f, 0.0f,
								1.0f, 0.0f,
								0.0f, 1.0f };
	glBindBuffer(GL_ARRAY_BUFFER, triangleID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, aBunchOfFloats, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

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
	glfwTerminate();

	return 0;
}