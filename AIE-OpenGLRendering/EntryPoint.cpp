#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Game.h"
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

	Game game;

	// main game loop
	while (!glfwWindowShouldClose(Renderer::GetWindow()))
	{
		Input::UpdateInput();
		game.Update();
		Renderer::Render();
	}

	// window has closed, clean up GLFW and exit
	Renderer::Shutdown();

	return 0;
}