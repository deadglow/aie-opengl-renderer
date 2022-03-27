#include "Game.h"
#include "Input.h"
#include "Renderer.h"
#include "glm/ext/matrix_transform.hpp"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Update()
{
	glm::vec2 mousedelta = Input::GetMouseDelta();

	camera += mousedelta * mouseSens;

	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-camera.x), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = rotationMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(-camera.y), glm::vec3(1.0f, 0.0f, 0.0f));

	rotationMatrix[3] = glm::vec4(Renderer::camera.transform.GetPosition(), 1.0f);

	Renderer::camera.transform.matrix = rotationMatrix;

	glm::vec3 moveinput = Input::GetMoveInput();
	float mult = 1.0f;
	if (Input::GetShift())
		mult = speedBoost;

	glm::vec3 appliedmove = (glm::vec3(0, 0, -1) * moveinput.z + glm::vec3(1, 0, 0) * moveinput.x + glm::vec3(0, 1, 0) * moveinput.y) * mult * (float)Renderer::GetDeltaTime();
	Renderer::camera.transform.Translate(appliedmove, false);

	PointLight* light = (PointLight*)Renderer::lights[1];
	light->position.x = glm::cos((float)glfwGetTime()) * 1;
	light->position.z = glm::sin((float)glfwGetTime()) * 1;
	//light->intensity = glm::sin((float)glfwGetTime()) * 0.5f + 0.5f;

}
