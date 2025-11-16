#include "game.h"
#include "rng.h"
#include <print>

namespace rr {

Game::Game(const std::string& title) {
    video_mode_ = sf::VideoMode(1920, 1080);
	window_center_ = sf::Vector2f(video_mode_.width / 2.0f, video_mode_.height / 2.0f);
    window_ = new sf::RenderWindow(video_mode_, title);

    player_ = new Player();
	zombie_ = new Zombie(Zombie::Type::chaser);
	constexpr int num_zombies = 500;
	for (int i = 0; i < num_zombies; i++) {
		auto zombie_type = static_cast<Zombie::Type>(Rng::singleton().pick(Zombie::num_types));
		auto zombie = new Zombie(static_cast<Zombie::Type>(zombie_type));
		float padding = 40;
		auto x = static_cast<int>(Rng::singleton().pick(padding, video_mode_.width - padding));
		auto y = static_cast<int>(Rng::singleton().pick(padding, video_mode_.height - padding));
		zombie->set_position(x, y);
		zombie->set_chase_target(player_);
		zombies_.push_back(zombie);
	}
}

Game::~Game() {
	for (auto zombie : zombies_) {
		delete zombie;
	}
	delete zombie_;
	delete player_;
    delete window_;
}

void Game::run() {
    setup();
	sf::Clock clock;
    while (running_) {
        handle_input();
		Game::delta_time = clock.restart();
        update();
        render();
    }
}

void Game::setup() {
	std::println("{}, {}", window_center_.x, window_center_.y);
	player_->position = window_center_;
	zombie_->set_position(player_->position.x - 200, player_->position.y - 300.0f);
	zombie_->set_chase_target(player_);
}

void Game::handle_input() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			running_ = false;
			window_->close();
			break;
		}
	}

	player_->move_intent.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	player_->move_intent.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	player_->move_intent.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	player_->move_intent.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

	auto mouse_pixels = sf::Mouse::getPosition(*window_);
	mouse_position_ = window_->mapPixelToCoords(mouse_pixels);
}

void Game::update() {
	// 每次都添加update, draw很烦呐
	player_->update(mouse_position_);
	zombie_->update();
	for (auto zombie : zombies_) {
		zombie->update();
	}
}

void Game::render() {
	window_->clear();

	window_->draw(player_->get_visual());
	window_->draw(zombie_->get_visual());
	for (auto zombie : zombies_) {
		window_->draw(zombie->get_visual());
	}

	window_->display();
}

} // End namespace rr