#include "game.h"
#include <SFML/Graphics.hpp>
#include <memory>

int main() {
    auto game = std::make_unique<rr::Game>("Zombie Survivors");
    game->loop();
    return 0;
}