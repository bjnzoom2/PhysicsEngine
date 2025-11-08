#pragma once
#include "gl2d.h"

class Object {
public:
	Object(glm::dvec2 pos, glm::dvec2 velo, float rad);

	glm::dvec2 position = { 400, 400 };
	glm::dvec2 velocity = {};
	float radius = 20;

	void render(gl2d::Renderer2D& renderer);

	void step(float deltatime);
};