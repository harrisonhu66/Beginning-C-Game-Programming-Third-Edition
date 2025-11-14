#include "game.h"
#include <print>

namespace rr {

Game::Game(const std::string title) {
    auto mode = sf::VideoMode(1920, 1080);
	window_center_ = sf::Vector2f(mode.width / 2.0f, mode.height / 2.0f);
    window_ = new sf::RenderWindow(mode, title);

    //player_ = new Player();
}

Game::~Game() {
    delete window_;
}

void Game::run() {
    setup();
	sf::Clock clock;
    while (is_running_) {
        handle_input();
		auto dt = clock.restart();
        update(dt);
        render();
    }
}

void Game::setup() {
	std::println("{}, {}", window_center_.x, window_center_.y);
    player_.spawn(window_center_);
}

void Game::handle_input() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			is_running_ = false;
			window_->close();
			break;
		}
	}

	player_.move_intent.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	player_.move_intent.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	player_.move_intent.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	player_.move_intent.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

void Game::update(sf::Time dt) {
	player_.update(dt);
}

void Game::render() {
	window_->clear();

	window_->draw(player_.get_visual());

	window_->display();
}

} // End namespace rr