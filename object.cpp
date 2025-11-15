#include "object.h"
#include <iostream>

Object::Object(glm::dvec2 pos, glm::dvec2 velo, float rad) : position(pos), velocity(velo), radius(rad) {}

void Object::render(gl2d::Renderer2D& renderer) {
	renderer.renderCircleOutline(position, radius, Colors_White, 2, 64);
}

void Object::collideWithLine(const glm::dvec2& linePoint, const glm::dvec2& normal, const glm::dvec2& perpendicularNormal, glm::dvec2& position, glm::dvec2& velocity, double radius, float wallLength, glm::dvec2 dir,
    float friction, float restitution) {
    glm::dvec2 n = normal;
    double distance = glm::dot(position - linePoint, n);
    double distanceAlongLine = glm::dot(position - linePoint, perpendicularNormal);
    glm::dvec2 point;

    if (distance < 0.0) {
        n = -n;
        distance = -distance;
    }

    if (glm::distance(position, linePoint) < glm::distance(position, linePoint + (double)wallLength * glm::normalize(dir))) {
        point = linePoint;
    }
    else {
        point = linePoint + (double)wallLength * glm::normalize(dir);
    }

    if (std::abs(distance) < radius) {
        if (distanceAlongLine + radius >= 0 && distanceAlongLine - radius <= wallLength) {
            if (distanceAlongLine + radius < 0.5 || distanceAlongLine - radius > wallLength - 0.5) {
                n = glm::normalize(position - point);
            }
            glm::dvec2 tangent(n.y, -n.x);

            double speedAlongNormal = glm::dot(velocity, n);
            double speedAlongTangent = glm::dot(velocity, tangent);

            if (speedAlongNormal <= 0.0) {
                velocity = (-speedAlongNormal * n) * (double)restitution + (speedAlongTangent * tangent) * double(1 - friction);

                if (n != glm::normalize(position - point)) {
                    double penetration = radius - std::abs(distance);
                    position += n * penetration;
                }
            }
        }
    }
}

void Object::collideWithObject(Object& otherObj, float friction, float restitution) {
    float distance = glm::distance(position, otherObj.position);
    glm::dvec2 normal = (position - otherObj.position) / (double)distance;

    if (distance <= radius + otherObj.radius) {
        glm::dvec2 tangent(normal.y, -normal.x);

        double relativeSpeedAlongNormal = glm::dot(velocity - otherObj.velocity, normal);
        double relativeSpeedAlongTangent = glm::dot(velocity - otherObj.velocity, tangent);

        glm::dvec2 force = relativeSpeedAlongTangent / 2 * tangent * (double)friction;
        force += relativeSpeedAlongNormal / 2 * normal * double(1 + restitution);

        velocity -= force;
        otherObj.velocity += force;

        double penetration = (radius + otherObj.radius) - distance;
        position += normal * penetration;
    }
}

void Object::step(float deltatime, std::vector<Wall>& walls, std::vector<Object>& objects, float friction, float restitution) {
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

    velocity.y += 400 * deltatime;
	position += velocity * (double)deltatime;

    for (auto& wall : walls) {
        double rad = glm::radians(wall.angle);
        glm::dvec2 dir = { std::cos(rad), std::sin(rad) };
        glm::dvec2 normal = glm::normalize(glm::dvec2(dir.y, dir.x));
        glm::dvec2 perpendicularNormal = glm::dvec2(normal.y, -normal.x);

        collideWithLine(wall.position, normal, perpendicularNormal, position, velocity, radius, wall.length, dir, friction, restitution);
    }

    for (int i = 0; i < objects.size(); i++) {
        if (this == &objects[i]) continue;
        collideWithObject(objects[i], friction, restitution);
    }
}