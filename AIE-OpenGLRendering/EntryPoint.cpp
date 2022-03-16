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
	GLuint colorsID;
	glGenBuffers(1, &triangleID);

	struct Vertex
	{
		float x, y;
		unsigned char r, g, b, a;
	};

	Vertex vertices[6];
	vertices[0] = { 0, 0,
					255, 0, 0, 255 };
	vertices[1] = { 1, 1,
					0, 255, 0, 255 };
	vertices[2] = { 1, 0,
					0, 0, 255, 255 };
	vertices[3] = { 0, 0,
					255, 0, 0, 255 };
	vertices[4] = { 1, 1,
					0, 255, 0, 255 };
	vertices[5] = { 0, 1,
					0, 0, 255, 255 };
	
	glBindBuffer(GL_ARRAY_BUFFER, triangleID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	shaders->UseShader("vertcolor");
	glDrawArrays(GL_TRIANGLES, 0, 6);
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