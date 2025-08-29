#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include <json.hpp>
#include <fstream>
using json = nlohmann::json;


#include "Enemy.hpp"
#include "Tower.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "Wave.hpp"

enum class GameState{
    MENU,
    PLAYING,
    GAME_OVER
};

struct EnemyData {
    float speed;
    int hp;
};


class Game {
public:
    Game();
    void run();
    

private:

    void processEvents();
    void update(float dt);
    void render();
    void loadLevel(const std::string& filename);
    void resetGame();
    void updateButtonPositions();

    

    bool isOnPath(const sf::Vector2f& pos);
    bool placingTower;

    //Mechanika fal
    void startNextWave();
    int currentWave = 0;
    bool waveInProgress = false;
    sf::RectangleShape startWaveButton;
    sf::Text startWaveText;
    
    sf::RectangleShape menuButton;
    sf::Text menuText;

    sf::RectangleShape restartButton;
    sf::Text restartText;

    sf::Text gameOverText;




    std::vector<Wave> waves;
    int enemiesSpawned=0;
    float spawnTimer = 0.f;


   
    GameState state;

    sf::RenderWindow window;
    sf::Font font;

    // UI elementy
    sf::Text goldText;
    sf::Text hpText;

    // Ghost wieży (pokazuje, gdzie postawisz wieżę)
    sf::CircleShape towerGhost;
    
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    sf::RectangleShape startButton;
    sf::Text startText;

    sf::RectangleShape exitButton;
    sf::Text exitText;



    std::vector<sf::Vector2f> path; // ścieżka dla wrogów

    sf::RectangleShape buyTowerButton;
    sf::Text buyTowerText;

    

    sf::Clock clock;
    
};
