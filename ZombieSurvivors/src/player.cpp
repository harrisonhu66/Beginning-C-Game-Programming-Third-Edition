#include "player.h"
#include "game.h"
#include <cmath>
namespace rr {

void Player::awake() {
	speed_ = 200.0f;
	sprite_.setTexture(TextureMap::singleton()["graphics/player.png"]);
	// center the origin
	sprite_.setOrigin(25, 25);
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
	}
}

const sf::Sprite& Player::get_visual() const { 
	return sprite_; 
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
