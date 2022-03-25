#pragma once
#include "glm/glm.hpp"


class Game
{
private:
	glm::vec2 camera = {0.0f, 0.0f};
	float mouseSens = 0.15f;
	float moveSpeed = 1.0f;
	float speedBoost = 10.0f;
public:
	Game();
	~Game();

	void Update();
};

