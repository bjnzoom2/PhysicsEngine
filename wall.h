#pragma once
#include "gl2d.h"

class Wall {
public:
	Wall(glm::dvec2 pos, float ang, float leng);

	glm::dvec2 position;
	float angle;
	float length;
	float width = 5;

	void render(gl2d::Renderer2D& renderer);
};