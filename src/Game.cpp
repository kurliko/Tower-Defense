#include "Game.hpp"
#include <iostream>

Game::Game() : window(sf::VideoMode(800,600), "Tower Defense"), player(1,200), placingTower(false){
    window.setFramerateLimit(60);
    state = GameState::MENU;

    if(!font.loadFromFile("../assets/Fonts/DejaVuSans.ttf")){
        std::cerr<<"Failed to load font\n";
    }


    currentWave = -1;
    waveInProgress = false;

    //Ładowanie levelu
    loadLevel("../assets/levels/level1.json");

    //Przyciski w menu

    // Start Button
    startButton.setSize({200, 50});
    startButton.setPosition(300, 200);
    startButton.setFillColor(sf::Color{0,128,0});

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(320, 215);

    //Game-Over tekst
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(220, 150);

    // Exit Button
    exitButton.setSize({200, 50});
    exitButton.setPosition(300, 300);
    exitButton.setFillColor(sf::Color::Red);

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(370, 315);



    // // Przycisk startu fali
    // startWaveButton.setSize({100, 40});
    // startWaveButton.setPosition(650, 80);
    // startWaveButton.setFillColor(sf::Color::Green);

    // startWaveText.setFont(font);
    // startWaveText.setString("Start Wave");
    // startWaveText.setCharacterSize(16);
    // startWaveText.setPosition(655, 85);


    // Dodajemy jedną wieżę (na sztywno)
    // towers.push_back(std::make_unique<Tower>(sf::Vector2f(400, 300)));

    // UI — złoto
    goldText.setFont(font);
    goldText.setCharacterSize(20);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(10, 10);
    goldText.setString("Gold: " + std::to_string(player.getGold()));

    // UI — HP
    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition(150, 10);
    hpText.setString("HP: " + std::to_string(player.getHP()));

    // "Duch" wieży
    towerGhost.setRadius(20);
    towerGhost.setOrigin(20, 20); // środek w kole
    towerGhost.setFillColor(sf::Color(0, 0, 255, 128));

    //Przycisk kupowania wieży
    buyTowerButton.setSize({100, 40});
    buyTowerButton.setPosition(650, 20);
    buyTowerButton.setFillColor(sf::Color::Blue);
    //Tekst kupowania wieży
    buyTowerText.setFont(font);
    buyTowerText.setString("Buy Tower");
    buyTowerText.setCharacterSize(16);
    buyTowerText.setPosition(655, 25);
    
    //Przycisk startu nowej fali
    startWaveButton.setSize({100, 40});
    startWaveButton.setPosition(525, 20);
    startWaveButton.setFillColor(sf::Color{0,128,0});
    //teskt dla przycisku startu fali
    startWaveText.setFont(font);
    startWaveText.setString("Start Wave");
    startWaveText.setCharacterSize(16);
    startWaveText.setPosition(530, 30);

    //Przycisk menu
    menuButton.setSize({100, 40});
    menuButton.setPosition(15, 550);
    menuButton.setFillColor(sf::Color::Yellow);
    //teskt dla przycisku menu
    menuText.setFont(font);
    menuText.setString("Menu");
    menuText.setCharacterSize(16);
    menuText.setFillColor(sf::Color::Black);
    menuText.setPosition(30, 560);


    //Przycisk restartu
    restartButton.setSize({150, 40});
    restartButton.setPosition(150, 550);
    restartButton.setFillColor(sf::Color::Red);
    //teskt dla przycisku restartu
    restartText.setFont(font);
    restartText.setString("Restart Game");
    restartText.setCharacterSize(16);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(165, 560);


}



void Game::update(float dt) {

    updateButtonPositions();
    // Update wież (dodają pociski)
    for (auto& t : towers) {
        t->update(dt, enemies, projectiles);
    }

    // Update pocisków
    for (auto& p : projectiles) p->update(dt);

    

    //Mechanika wrógów
    for (auto it = enemies.begin(); it != enemies.end();) {
    (*it)->update(dt);

    // Usuwanie martwych wrogów
    if ((*it)->isDead()) {
        player.addGold(10);
        it = enemies.erase(it);
    }
    else if ((*it)->reachedEnd()) {
        player.takeDamage(1);  // zabiera życie
        it = enemies.erase(it);
    }
    else {
        ++it;
    }
}


    // Usuwamy pociski, które trafiły
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](auto& p) { return p->isDestroyed(); }),
        projectiles.end()
    );

    // np. za zabicie wroga:
    // if (enemyDead) player.addGold(10);

    goldText.setString("Gold: " + std::to_string(player.getGold()));
    hpText.setString("HP: " + std::to_string(player.getHP()));

    if(player.getHP()<=0){
        state=GameState::GAME_OVER;
    }

    //Trwa Fala
    if (waveInProgress) {
        Wave& wave = waves[currentWave];
        spawnTimer += dt;

        // spawn przeciwników co spawnDelay
        if (enemiesSpawned < wave.count && spawnTimer >= wave.spawnDelay) {
            enemies.push_back(std::make_unique<Enemy>(path, wave.speed, wave.hp));
            enemiesSpawned++;
            spawnTimer = 0.f;
        }

        // fala skończona -> przechodzimy dalej
        if (enemiesSpawned == wave.count && enemies.empty()) {
            waveInProgress = false;
            player.addGold(50); // nagroda
        }
        }
        

    


}

void Game::run(){
    while(window.isOpen()){
        processEvents();
        float dt = clock.restart().asSeconds();
        update(dt);
        render();
    }
}


void Game::loadLevel(const std::string& filename) {
    std::ifstream f(filename);
    json data;
    f >> data;

    // Wczytywanie ścieżki
    path.clear();
    for (auto& p : data["path"]) {
        path.emplace_back(p[0], p[1]);
    }

    // Wczytywanie fal
    waves.clear();
    for (auto& w : data["waves"]) {
        Wave wave;
        wave.count = w["count"];
        wave.speed = w["speed"];
        wave.hp = w["hp"];
        waves.push_back(wave);
    }
}


bool Game::isOnPath(const sf::Vector2f& pos) {
    // Zakładam że path to vector punktów połączonych linią
    float radius = 30.f; // "grubość" ścieżki

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        sf::Vector2f a = path[i];
        sf::Vector2f b = path[i + 1];

        // obliczamy odległość punktu pos od odcinka AB
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float length2 = dx * dx + dy * dy;

        float t = ((pos.x - a.x) * dx + (pos.y - a.y) * dy) / length2;
        t = std::max(0.f, std::min(1.f, t));

        sf::Vector2f projection(a.x + t * dx, a.y + t * dy);

        float dist2 = (pos.x - projection.x) * (pos.x - projection.x)
                    + (pos.y - projection.y) * (pos.y - projection.y);

        if (dist2 < radius * radius)
            return true; // kliknięcie na ścieżce
    }

    return false;
}

void Game::updateButtonPositions() {
    if (state == GameState::PLAYING) {
        menuButton.setPosition(15, 550);
        menuText.setPosition(30, 560);

        restartButton.setPosition(150, 550);
        restartText.setPosition(165, 560);
    }
    else if (state == GameState::GAME_OVER) {
        menuButton.setPosition(350, 250); // np. środek ekranu
        menuText.setPosition(370, 260);

        restartButton.setPosition(350, 300);
        restartText.setPosition(370, 310);
    }
    else if (state == GameState::MENU) {
        // Tutaj możesz ustawić inne przyciski menu głównego
    }
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
            if (placingTower && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (player.getGold() >= Tower::cost && !isOnPath(pos)) {
                    towers.push_back(std::make_unique<Tower>(pos));
                    player.addGold(-Tower::cost);
                    placingTower = false;
                }
            }
        }

    if (event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

    if (state == GameState::MENU) {
        if (startButton.getGlobalBounds().contains(mousePos)) {
            // startujemy grę
            state = GameState::PLAYING;
            render();
        } else if (exitButton.getGlobalBounds().contains(mousePos)) {
            window.close(); // wychodzimy z aplikacji
        }
    }
    else if(state == GameState::GAME_OVER){
        if (menuButton.getGlobalBounds().contains(mousePos)) {
            state=GameState::MENU;
        }
        if (restartButton.getGlobalBounds().contains(mousePos)) {
            resetGame();
        }
    }

    else if(state == GameState::PLAYING){
        if (menuButton.getGlobalBounds().contains(mousePos)) {
            state=GameState::MENU;
        }
        if (restartButton.getGlobalBounds().contains(mousePos)) {
            resetGame();
        }
    }

    // if (menuButton.getGlobalBounds().contains(mousePos)) {
    //     state=GameState::MENU;
    // }

    // if (restartButton.getGlobalBounds().contains(mousePos)) {
    //     resetGame();
    // }
    

    // kliknięcie w przycisk kupna wieży
    if (buyTowerButton.getGlobalBounds().contains(mousePos)) {
        if (player.getGold() >= Tower::cost) {
            placingTower = true;
        }
    }

    }

    //Przycisn staru nowej fali
    if (event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

    if (startWaveButton.getGlobalBounds().contains(mousePos)) {
        if (!waveInProgress) {
            startNextWave();
        }
    }
}


}


void Game::startNextWave() {
    currentWave++;
    if (currentWave >= waves.size()) {
        std::cout << "All waves completed!\n";
        return;
    }



    enemiesSpawned = 0;
    spawnTimer = 0.f; // pierwszy od razu
    waveInProgress = true;
}


void Game::render() {
    window.clear(sf::Color::Black);
    

    window.clear(sf::Color::Black);

    if (state == GameState::MENU) {
        window.draw(startButton);
        window.draw(startText);
        window.draw(exitButton);
        window.draw(exitText);
    } 
    else if (state == GameState::PLAYING) {
        window.clear(sf::Color::Black);

        //Przycisk menu
        window.draw(menuButton);
        window.draw(menuText);

        //Przycisk restartu
        window.draw(restartButton);
        window.draw(restartText);

        // Rysowanie ścieżki
            sf::VertexArray pathLine(sf::LineStrip, path.size());
        for (size_t i = 0; i < path.size(); i++) {
            pathLine[i].position = path[i];
            pathLine[i].color = sf::Color::Yellow;
        }
        window.draw(pathLine);
        // rysujemy mapę, wrogów, wieże, UI itp.
        // Rysowanie wrogów
        for (auto& e : enemies) e->draw(window);

        // Rysowanie wież
        for (auto& t : towers) t->draw(window);

        // Rysowanie pocisków
        for (auto& p : projectiles) p->draw(window);

        // Stawianie wieży
        if (placingTower) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
            towerGhost.setPosition(worldPos);
            window.draw(towerGhost);
        }

        // rysowanie UI
        goldText.setString("Gold: " + std::to_string(player.getGold()));
        window.draw(goldText);

        hpText.setString("HP: " + std::to_string(player.getHP()));
        window.draw(hpText);

        window.draw(buyTowerButton);
        window.draw(buyTowerText);

        window.draw(startWaveButton);
        window.draw(startWaveText);
        if (placingTower) window.draw(towerGhost);

        
    }
    else if (state == GameState::GAME_OVER) {
        window.clear(sf::Color::Black);
        updateButtonPositions();
        window.draw(gameOverText);

        window.draw(restartButton);
        window.draw(restartText);

        window.draw(menuButton);
        window.draw(menuText);

        // rysujemy ekran GameOver + przycisk Exit (wraca do startMenu)
    }

    window.display();

}

void Game::resetGame() {
    // wyczyść wrogów i pociski
    enemies.clear();
    projectiles.clear();
    towers.clear();

    // zresetuj fale
    currentWave = -1;
    enemiesSpawned = 0;
    spawnTimer = 0.f;
    waveInProgress = false;

    // zresetuj gracza
    player.setGold(200);   // np. startowe złoto
    player.setHp(100);   // np. startowe życie

    // ustaw stan gry na Playing
    state = GameState::PLAYING;
    loadLevel("../assets/levels/level1.json");
}
