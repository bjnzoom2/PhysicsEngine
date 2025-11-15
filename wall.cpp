#include "wall.h"
#include <iostream>

Wall::Wall(glm::dvec2 pos, float ang, float leng) : position(pos), angle(ang), length(leng) {}
Wall::Wall(glm::dvec2 startPos, glm::dvec2 endPos) : position(startPos) {
	double dx = endPos.x - position.x;
	double dy = endPos.y - position.y;
	angle = glm::degrees(atan2(-dy, dx));
	length = glm::distance(position, endPos);
}

void Wall::render(gl2d::Renderer2D& renderer) {
	renderer.renderLine(position, angle, length, Colors_White, width);
}