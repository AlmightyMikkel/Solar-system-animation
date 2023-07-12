#pragma once

#include <glm/glm.hpp>

class Light {
public:
	void randomize();
	glm::vec3 position{ 1.0f, 1.0f, 1.0f };
	glm::vec3 color{ 0.2f, 0.2f, 0.2f };
};