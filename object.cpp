#include "object.h"

Object::Object(glm::dvec2 pos, glm::dvec2 velo, float rad) : position(pos), velocity(velo), radius(rad) {};

void Object::render(gl2d::Renderer2D& renderer) {
	renderer.renderCircleOutline(position, radius, Colors_White, 2, 64);
}

void Object::step(float deltatime, Wall wall) {
	if (position.y + radius >= 800) { if (velocity.y > 0) velocity.y = -velocity.y; }
	if (position.y - radius <= 0) { if (velocity.y < 0) velocity.y = -velocity.y; }
	if (position.x + radius >= 800) { if (velocity.x > 0) velocity.x = -velocity.x; }
	if (position.x - radius <= 0) { if (velocity.x < 0) velocity.x = -velocity.x; }

	position += velocity * (double)deltatime;

    glm::dvec2 dir = glm::dvec2(std::cos(wall.angle), std::sin(wall.angle));

    glm::dvec2 p1 = wall.position;
    glm::dvec2 p2 = wall.position + dir * (double)wall.length;

    glm::dvec2 seg = p2 - p1;
    double segLen2 = glm::dot(seg, seg);

    double t = 0.0;
    if (segLen2 > 0.0) {
        t = glm::dot(position - p1, seg) / segLen2;
        t = std::clamp(t, 0.0, 1.0);
    }

    glm::dvec2 closest = p1 + seg * t;
    glm::dvec2 diff = position - closest;
    double dist2 = glm::dot(diff, diff);

    if (dist2 < (double)radius * (double)radius) {
        double dist = std::sqrt(dist2);
        glm::dvec2 normal;

        if (dist > 1e-9) {
            normal = diff / dist;
        }
        else {
            normal = glm::dvec2(-seg.y, seg.x);
            double nlen = glm::length(normal);
            if (nlen > 1e-9) normal /= nlen;
            else normal = glm::dvec2(0.0, 1.0);
        }

        double penetration = (double)radius - dist;
        position += normal * penetration;

        double velAlong = glm::dot(velocity, normal);
        if (velAlong < 0.0) {
            const double bounce = 1;
            velocity = velocity - (1.0 + bounce) * velAlong * normal;
        }
    }
}