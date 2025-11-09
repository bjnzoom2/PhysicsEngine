#pragma once
#include "gl2d.h"
#include "wall.h"

class Object {
public:
	Object(glm::dvec2 pos, glm::dvec2 velo, float rad);

	glm::dvec2 position = { 400, 400 };
	glm::dvec2 velocity = {};
	float radius = 20;

	void render(gl2d::Renderer2D& renderer);

	void collideWithLine(const glm::dvec2& linePoint, const glm::dvec2& normal, glm::dvec2& position, glm::dvec2& velocity, double radius);

	void step(float deltatime, std::vector<Wall> walls);
};