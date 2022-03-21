#include "Input.h"
#include "Renderer.h"
#include "ShaderLoader.h"
#include "glfw3.h"

GLFWwindow* Input::window = nullptr;
glm::dvec2 Input::mousePos;
glm::dvec2 Input::lastMousePos;
glm::dvec2 Input::mouseDelta;
glm::vec3 Input::moveinput;

void Input::Initialise(GLFWwindow* window_init)
{
	window = window_init;
	glfwSetKeyCallback(window, OnKey);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::UpdateInput()
{
	// poll for input and other events
	glfwPollEvents();

	glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
	
	mouseDelta = mousePos - lastMousePos;
	lastMousePos = mousePos;
}

glm::dvec2 Input::GetMousePos()
{
	return mousePos;
}

glm::dvec2 Input::GetMouseDelta()
{
	return mouseDelta;
}

glm::vec3 Input::GetMoveInput()
{
	return moveinput;
}

void Input::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R)
		{
			ShaderLoader::InitialiseShaders();
		}

		switch (key)
		{
		case GLFW_KEY_W:
			moveinput.z += 1;
			break;
		case GLFW_KEY_S:
			moveinput.z -= 1;
			break;

		case GLFW_KEY_D:
			moveinput.x += 1;
			break;
		case GLFW_KEY_A:
			moveinput.x -= 1;
			break;
		case GLFW_KEY_E:
			moveinput.y += 1;
			break;
		case GLFW_KEY_Q:
			moveinput.y -= 1;
			break;
		}
	}
	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			moveinput.z -= 1;
			break;
		case GLFW_KEY_S:
			moveinput.z += 1;
			break;

		case GLFW_KEY_D:
			moveinput.x -= 1;
			break;
		case GLFW_KEY_A:
			moveinput.x += 1;
			break;
		case GLFW_KEY_E:
			moveinput.y -= 1;
			break;
		case GLFW_KEY_Q:
			moveinput.y += 1;
			break;
		}
	}
}