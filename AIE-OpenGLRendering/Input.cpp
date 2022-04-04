#include "Input.h"
#include "Renderer.h"
#include "ShaderLoader.h"
#include "PostProcessing.h"
#include "GLFW/glfw3.h"

GLFWwindow* Input::window = nullptr;
glm::dvec2 Input::mousePos;
glm::dvec2 Input::lastMousePos;
glm::dvec2 Input::mouseDelta;
glm::vec3 Input::moveinput;
bool Input::shift = false;
bool Input::mouseLocked = false;

void Input::Initialise(GLFWwindow* window_init)
{
	window = window_init;
	glfwSetKeyCallback(window, OnKey);
}

void Input::UpdateInput()
{
	// poll for input and other events
	glfwPollEvents();

	glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
	
	if (mouseLocked)
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

bool Input::GetShift()
{
	return shift;
}

void Input::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R)
		{
			ShaderLoader::InitialiseShaders();
			PostProcessing::Initialise();
			
		}
		if (key == GLFW_KEY_SLASH)
		{
			mouseLocked = !mouseLocked;
			if (mouseLocked)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if (key == GLFW_KEY_GRAVE_ACCENT)
			Renderer::drawDebug = !Renderer::drawDebug;

		if (key == GLFW_KEY_LEFT_SHIFT)
			shift = true;

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
		if (key == GLFW_KEY_LEFT_SHIFT)
			shift = false;

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