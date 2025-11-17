#include "game.h"
#include "rng.h"
#include <print>

namespace rr {

Game::Game(const std::string& title) {
    video_mode_ = sf::VideoMode(1920, 1080);
	window_center_ = sf::Vector2f(video_mode_.width / 2.0f, video_mode_.height / 2.0f);
    window_ = std::make_unique<sf::RenderWindow>(video_mode_, title);
	main_camera_ = sf::View(sf::FloatRect(0, 0, video_mode_.width, video_mode_.height));
	is_running_ = true;
}

void Game::setup() {
	arena_ = std::make_unique<Arena>(sf::IntRect(0, 0, video_mode_.width * 2, video_mode_.height * 2), 50);

    player_ = std::make_unique<Player>();
	player_->awake();
	player_->start(window_.get());
	std::println("{}, {}", window_center_.x, window_center_.y);
	player_->position = window_center_;

	constexpr int num_zombies = 500;
	for (int i = 0; i < num_zombies; i++) {
		auto zombie_type = static_cast<Zombie::Type>(Rng::singleton().pick(Zombie::num_types));
		auto zombie = std::make_unique<Zombie>(static_cast<Zombie::Type>(zombie_type));
		zombie->awake();
		zombie->start(player_.get());

		float padding = 40;
		auto x = static_cast<int>(Rng::singleton().pick(padding, arena_->get_region().width - padding));
		auto y = static_cast<int>(Rng::singleton().pick(padding, arena_->get_region().height - padding));
		zombie->set_position(x, y);
		zombies_.push_back(std::move(zombie));
	}
}

void Game::loop() {
    setup();
	sf::Clock clock;
    while (is_running_) {
        process_inputs();
		Game::delta_time = clock.restart();
        update();
        render();
    }
}

void Game::process_inputs() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			is_running_ = false;
			window_->close();
			break;
		}
	}
}

void Game::update() {
	// 每次都添加update, draw很烦呐
	player_->update();
	main_camera_.setCenter(player_->position);
	for (auto& zombie : zombies_) {
		zombie->update();
	}
}

void Game::render() {
	window_->clear();

	const auto& arena_visual = arena_->get_visual();
	window_->draw(arena_visual.vertices, arena_visual.texture);

	window_->setView(main_camera_);
	window_->draw(player_->get_visual());
	for (auto& zombie : zombies_) {
		window_->draw(zombie->get_visual());
	}

	window_->display();
}

} // End namespace rr