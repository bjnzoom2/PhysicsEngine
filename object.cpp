#include "object.h"

Object::Object(glm::dvec2 pos, glm::dvec2 velo, float rad) : position(pos), velocity(velo), radius(rad) {};

void Object::render(gl2d::Renderer2D& renderer) {
	renderer.renderCircleOutline(position, radius, Colors_White, 2, 64);
}

void Object::step(float deltatime) {
	if (position.y + radius >= 800) {
		if (velocity.y > 0) {
			velocity.y = -velocity.y;
		}
	}
	if (position.y - radius <= 0) {
		if (velocity.y < 0) {
			velocity.y = -velocity.y;
		}
	}
	if (position.x + radius >= 800) {
		if (velocity.x > 0) {
			velocity.x = -velocity.x;
		}
	}
	if (position.x - radius <= 0) {
		if (velocity.x < 0) {
			velocity.x = -velocity.x;
		}
	}

	position += velocity * (double)deltatime;
}