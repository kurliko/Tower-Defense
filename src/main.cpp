#include "Game.hpp"
#include <ctime>
#include <cstdlib> // std::getenv

int main() {
    std::srand(std::time(nullptr)); // dla losowania
    Game game;
    game.run();
    return 0;
}