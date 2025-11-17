#include "game.h"
#include <SFML/Graphics.hpp>

int main() {
    auto game = new rr::Game("Zombie Survivors");
    game->loop();
    delete game;
    return 0;
}