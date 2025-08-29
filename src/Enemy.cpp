#include "Enemy.hpp"

Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed, int hp)
    : path(path), speed(speed), hp(hp), pathIndex(0) 
{
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(15,15);
    if (!path.empty())
        shape.setPosition(path[0]);
}

void Enemy::update(float dt) {
    if (pathIndex +1 >= path.size()) return;

    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f target = path[pathIndex + 1];
    sf::Vector2f dir = target - pos;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0) dir /= length;

    shape.move(dir * speed * dt);

    if (std::abs(pos.x - target.x) < 5 && std::abs(pos.y - target.y) < 5) {
        pathIndex++;
    }
}


void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const{
    return shape.getPosition();
}

void Enemy::takeDamage(int dmg){
    hp-=dmg;
}

bool Enemy::reachedEnd() const {
    return pathIndex +1 >= path.size();
    }
