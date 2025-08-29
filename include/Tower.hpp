#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.hpp"
#include "Projectile.hpp"

class Tower {
public:
    // Tower(sf::Vector2f pos, float range, int damage, float fireRate);
    Tower(sf::Vector2f pos);
    void update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles);
    void draw(sf::RenderWindow& window);
    int getCost(){return cost;}
    static const int cost = 50;

private:
    
    sf::CircleShape shape;
    float range;
    int damage;
    float fireRate;
    float timeSinceLastShot;
    float fireCooldown;
};
