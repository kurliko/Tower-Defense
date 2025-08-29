#include "Tower.hpp"
#include <cmath>

Tower::Tower(sf::Vector2f pos) : range(300.f), fireRate(1.f), fireCooldown(0.f) {
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(20, 20);
    shape.setPosition(pos);
}

void Tower::update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies,
                   std::vector<std::unique_ptr<Projectile>>& projectiles) {
    fireCooldown -= dt;
    if (fireCooldown > 0) return;

    // Znajdź najbliższego wroga
    Enemy* target = nullptr;
    float minDist = range;

    for (auto& e : enemies) {
        float dx = e->getPosition().x - shape.getPosition().x;
        float dy = e->getPosition().y - shape.getPosition().y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < minDist) {
            minDist = dist;
            target = e.get();
        }
    }

    if (target) {
        projectiles.push_back(std::make_unique<Projectile>(
            shape.getPosition(), target, 300.f, 10));
        fireCooldown = 1.f / fireRate;
    }
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
