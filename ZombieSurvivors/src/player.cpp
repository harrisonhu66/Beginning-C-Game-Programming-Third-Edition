#include "player.h"
#include "game.h"
#include <cmath>
namespace rr {
Player::Player() {
	// center the origin
	sprite_.setOrigin(25, 25);
}

void Player::update(const sf::Vector2f& mouse_position) {
	handle_movement();
	handle_rotation(mouse_position);
}

void Player::handle_movement() {
	auto dir = sf::Vector2f(0, 0);
	if (move_intent.right) {
		dir.x += 1;
	}
	if (move_intent.left) {
		dir.x -= 1;
	}
	if (move_intent.down) {
		dir.y += 1;
	}
	if (move_intent.up) {
		dir.y -= 1;
	}
	if (dir != sf::Vector2f(0, 0)) {
		dir = dir / std::sqrt(dir.x * dir.x + dir.y * dir.y);
	}
	position += Game::delta_time.asSeconds() * speed_ * dir;
	sprite_.setPosition(position);
}

void Player::handle_rotation(const sf::Vector2f& mouse_position) {
	float degree = 180.0f / std::acos(-1.0f) * std::atan2(mouse_position.y - position.y, mouse_position.x - position.x);
	sprite_.setRotation(degree);
}

} // End namespace rr
