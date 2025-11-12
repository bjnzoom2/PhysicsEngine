#pragma once
#include "object.h"

class Spring {
public:
	Spring(Object& springNode1, Object& springNode2, float springLength, float springForce);

	Object* node1;
	Object* node2;
	float length;
	float force;

	void render(gl2d::Renderer2D& renderer);

	void step(float deltatime);
};