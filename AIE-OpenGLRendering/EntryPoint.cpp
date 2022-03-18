#pragma once
#include "Renderer.h"
#include "Input.h"

int main()
{
	int initStatus = Renderer::Initialise();
	if (initStatus != 0)
	{
		Renderer::Shutdown();
		return initStatus;
	}
	Input::Initialise(Renderer::GetWindow());

	// main game loop
	while (Renderer::GetShaderLoader()->GetShaderStateOkay() && !glfwWindowShouldClose(Renderer::GetWindow()))
	{
		Input::UpdateInput();
		Renderer::Render();
	}

	// window has closed, clean up GLFW and exit
	Renderer::Shutdown();

	return 0;
}