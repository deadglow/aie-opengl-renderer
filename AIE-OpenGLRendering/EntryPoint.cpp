#pragma once
#include "Renderer.h"
#include "Input.h"
#include <iostream>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int initStatus = Renderer::Initialise();
	if (initStatus != 0)
	{
		Renderer::Shutdown();
		return initStatus;
	}
	Input::Initialise(Renderer::GetWindow());

	Renderer::Start();

	// main game loop
	while (ShaderLoader::GetShaderStateOkay() && !glfwWindowShouldClose(Renderer::GetWindow()))
	{
		Input::UpdateInput();
		Renderer::Render();
	}

	// window has closed, clean up GLFW and exit
	Renderer::Shutdown();

	return 0;
}