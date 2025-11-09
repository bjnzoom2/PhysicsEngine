#include "wall.h"

Wall::Wall(glm::dvec2 pos, float ang, float leng) : position(pos), angle(ang), length(leng) {};

void Wall::render(gl2d::Renderer2D& renderer) {
	renderer.renderLine(position, angle, length, Colors_White, width);
}