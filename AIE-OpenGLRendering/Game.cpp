#include "Game.h"
#include "Input.h"
#include "Renderer.h"
#include "ext/matrix_transform.hpp"

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

	rotationMatrix[3] = Renderer::camera.transform[3];

	Renderer::camera.transform = rotationMatrix;

	glm::vec3 moveinput = Input::GetMoveInput();
	Renderer::camera.transform = Renderer::camera.transform * glm::translate(glm::mat4(1.0f), (glm::vec3(0, 0, 1) * -moveinput.z + glm::vec3(1, 0, 0) * moveinput.x + glm::vec3(0, 1, 0) * moveinput.y) * moveSpeed * (float)Renderer::GetDeltaTime());

}
