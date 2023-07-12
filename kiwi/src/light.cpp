#include "light.hpp"



void Light::randomize() {
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	this->position = { rand() % 360, rand() % 360, rand() % 360 };
	this->color = { r, g,b };


}