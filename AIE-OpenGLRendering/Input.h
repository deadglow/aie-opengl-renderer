#pragma once
#include "glfw3.h"
#include "glm.hpp"

class Input
{
private:
	static GLFWwindow* window;
	static glm::dvec2 mousePos;
	static void OnKey (GLFWwindow* window, int key, int scancode, int action, int mods);
	Input() = default;
	~Input() = default;
public:
	static void Initialise(GLFWwindow* window_init);
	static void UpdateInput();
};