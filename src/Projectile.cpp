#include "Projectile.hpp"
#include <cmath>

Projectile::Projectile(sf::Vector2f start, Enemy* target, float speed, int damage)
    : target(target), speed(speed), damage(damage), destroyed(false) {
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(5, 5);
    shape.setPosition(start);
}

void Projectile::update(float dt) {
    if (!target || target->isDead()) {
        destroyed = true;
        return;
    }

    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f dir = targetPos - pos;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0) return;

    dir /= length;
    shape.move(dir * speed * dt);

    if (length < 10) { // trafienie
        target->takeDamage(damage);
        destroyed = true;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
