#include "player.h"
#include "game.h"
#include "bullet_registry.h"
#include <cmath>
namespace rr {

void Player::awake() {
	speed_ = 200.0f;
	sprite_.setTexture(TextureMap::singleton()["graphics/player.png"]);
	auto sprite_size = sprite_.getGlobalBounds().getSize();
	auto sprite_center = sf::Vector2f(sprite_size.x / 2.0f, sprite_size.y / 2.0f);
	// enter the origin
	sprite_.setOrigin(sprite_center.x, sprite_center.y);
	fire_rate = 10.0f; 

	auto collider_size = sf::Vector2f(sprite_size.x - 22, sprite_size.y - 22);
	collider_ = std::make_unique<BoxCollider>(collider_size);
}

void Player::start(const sf::RenderWindow* window) {
	window_ = window;
}

void Player::update() {
	handle_movement();

	if (window_) {
		auto mouse_pixel = sf::Mouse::getPosition(*window_);
		auto mouse_position = window_->mapPixelToCoords(mouse_pixel);
		look_at(mouse_position);

		handle_fire(mouse_position);
	}

	collider_->update(position);
}

void Player::render(sf::RenderWindow* window) {
	// TODO It's really annoying to handle write all the draws/updates
	window->draw(sprite_);
	collider_->render(window);
}

void Player::handle_fire(const sf::Vector2f& mouse_position) {
	bool is_fire_button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool is_fire_in_cd = Game::time.asMilliseconds() - last_fire_time.asMilliseconds() < 1000 / fire_rate;
	if (is_fire_button_pressed && !is_fire_in_cd) {
		auto dir = sf::Vector2f(mouse_position.x - position.x, mouse_position.y - position.y);
		if (dir != sf::Vector2f(0, 0)) {
			auto length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
			dir /= length;
		}
		fire(dir);
		last_fire_time = Game::time;
	}
}


void Player::fire(const sf::Vector2f& dir) {
	BulletRegistry::singleton().spawn(position, dir, window_);
}

void Player::handle_movement() {
	auto dir = sf::Vector2f(0, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		dir.x += 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		dir.x -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		dir.y += 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		dir.y -= 1;
	}
	if (dir != sf::Vector2f(0, 0)) {
		dir = dir / std::sqrt(dir.x * dir.x + dir.y * dir.y);
	}
	position += Game::delta_time.asSeconds() * speed_ * dir;
	sprite_.setPosition(position);
}

// Just a semi-abstraction
void Player::look_at(const sf::Vector2f& target) {
	float degree = 180.0f / std::acos(-1.0f) * std::atan2(target.y - position.y, target.x - position.x);
	sprite_.setRotation(degree);
}

} // End namespace rr
