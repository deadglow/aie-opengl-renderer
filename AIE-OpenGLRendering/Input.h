#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Input
{
private:
	static GLFWwindow* window;
	static glm::dvec2 mousePos;
	static glm::dvec2 lastMousePos;
	static glm::dvec2 mouseDelta;
	static glm::vec3 moveinput;
	static bool shift;
	static bool mouseLocked;
	static void OnKey (GLFWwindow* window, int key, int scancode, int action, int mods);
	Input() = default;
	~Input() = default;
public:
	static void Initialise(GLFWwindow* window_init);
	static void UpdateInput();

	static glm::dvec2 GetMousePos();
	static glm::dvec2 GetMouseDelta();
	static glm::vec3 GetMoveInput();
	static bool GetShift();
};