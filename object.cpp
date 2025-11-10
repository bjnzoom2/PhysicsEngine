#include "object.h"
#include <iostream>

Object::Object(glm::dvec2 pos, glm::dvec2 velo, float rad) : position(pos), velocity(velo), radius(rad) {};

void Object::render(gl2d::Renderer2D& renderer) {
	renderer.renderCircleOutline(position, radius, Colors_White, 2, 64);
}

void Object::collideWithLine(const glm::dvec2& linePoint, const glm::dvec2& normal, const glm::dvec2& perpendicularNormal, glm::dvec2& position, glm::dvec2& velocity, double radius, float wallLength) {
    glm::dvec2 n = normal;
    double distance = glm::dot(position - linePoint, n);
    double distanceAlongLine = glm::dot(position - linePoint, perpendicularNormal);

    if (distance < 0.0) {
        n = -n;
        distance = -distance;
    }

    if (distanceAlongLine + radius > 0 && distanceAlongLine - radius < wallLength) {
        if (std::abs(distance) < radius)
        {
            glm::dvec2 tangent(normal.y, -normal.x);

            double speedAlongNormal = glm::dot(velocity, n);
            double speedAlongTangent = glm::dot(velocity, tangent);

            if (speedAlongNormal <= 0.0)
            {
                velocity = (-speedAlongNormal * n) + (speedAlongTangent * tangent);

                double penetration = radius - std::abs(distance);
                position += n * penetration;
            }
        }
    }

    std::cout << distanceAlongLine << '\n';
}

void Object::step(float deltatime, std::vector<Wall> walls) {
	if (position.y + radius >= 800) { if (velocity.y > 0) velocity.y = -velocity.y; }
	if (position.y - radius <= 0) { if (velocity.y < 0) velocity.y = -velocity.y; }
	if (position.x + radius >= 800) { if (velocity.x > 0) velocity.x = -velocity.x; }
	if (position.x - radius <= 0) { if (velocity.x < 0) velocity.x = -velocity.x; }

    velocity.y += 400 * deltatime;
	position += velocity * (double)deltatime;

    for (auto& wall : walls) {
        double rad = glm::radians(wall.angle);
        glm::dvec2 dir = { std::cos(rad), std::sin(rad) };
        glm::dvec2 normal = glm::normalize(glm::dvec2(dir.y, dir.x));
        glm::dvec2 perpendicularNormal = glm::dvec2(normal.y, -normal.x);

        collideWithLine(wall.position, normal, perpendicularNormal, position, velocity, radius, wall.length);
        
        // std::cout << normal.x << ' ' << normal.y << '\n';
    }
}