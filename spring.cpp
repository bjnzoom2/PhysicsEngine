#include "spring.h"
#include <iostream>

Spring::Spring(Object& springNode1, Object& springNode2, float springLength, float springForce) : node1(&springNode1), node2(&springNode2), length(springLength), force(springForce) {};

void Spring::render(gl2d::Renderer2D& renderer) {
	renderer.renderLine(node1->position, node2->position, Colors_White, 4);
}

void Spring::step(float deltatime) {
	glm::dvec2 dist = (node1->position) - (node2->position);
	float distance = glm::distance(node1->position, node2->position);

	glm::dvec2 impulse = -dist * (1.0 - length / distance) / 2.0 * (double)force * (double)deltatime;

	node1->velocity += impulse;
	node2->velocity -= impulse;
}