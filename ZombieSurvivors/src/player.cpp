#include "player.h"
#include <cmath>
namespace rr {
Player::Player() {
	// center the origin
	sprite_.setOrigin(25, 25);
}
void Player::spawn(float x, float y) {
	position_ = sf::Vector2f(x, y);
}

void Player::update(const sf::Time& dt) {
	auto dir = sf::Vector2f(0, 0);
	if (this->move_intent.right) {
		dir.x += 1;
	}
	if (this->move_intent.left) {
		dir.x -= 1;
	}
	if (this->move_intent.down) {
		dir.y += 1;
	}
	if (this->move_intent.up) {
		dir.y -= 1;
	}
	if (dir != sf::Vector2f(0, 0)) {
		dir = dir / std::sqrt(dir.x * dir.x + dir.y * dir.y);
	}
	position_ += dt.asSeconds() * speed_ * dir;
	sprite_.setPosition(position_);
}

} // End namespace rr
