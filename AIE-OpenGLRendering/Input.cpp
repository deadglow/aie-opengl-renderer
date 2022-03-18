#include "Input.h"
#include "Renderer.h"
#include "ShaderLoader.h"
#include "glfw3.h"

GLFWwindow* Input::window = nullptr;
glm::dvec2 Input::mousePos;

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
}

void Input::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R)
		{
			Renderer::GetShaderLoader()->InitialiseShaders();
		}
	}
}