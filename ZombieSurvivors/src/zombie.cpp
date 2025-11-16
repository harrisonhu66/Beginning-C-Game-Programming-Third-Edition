#include "texture_map.h"
#include "game.h"
#include "zombie.h"
#include <cmath>

namespace rr {

Zombie::Zombie(Type type) {
	switch (type) {
	case Type::crawler:
		speed_ = 20;
		sprite_.setTexture(TextureMap::singleton()["graphics/crawler.png"]);
		sprite_.setOrigin(50 / 2.0f, 50 / 2.0f);
		break;
	case Type::chaser:
		speed_ = 40;
		sprite_.setTexture(TextureMap::singleton()["graphics/chaser.png"]);
		sprite_.setOrigin(55 / 2.0f, 55 / 2.0f);
		break;
	case Type::bloater:
		speed_ = 80;
		sprite_.setTexture(TextureMap::singleton()["graphics/bloater.png"]);
		sprite_.setOrigin(75 / 2.0, 75 / 2.0f);
		break;
	}
}

void Zombie::update() {
	chase_target();
}

void Zombie::chase_target() {
	if (player_ == nullptr) {
		return;
	}
	auto dir = player_->position - position_;
	if (dir != sf::Vector2f(0, 0)) {
		dir = dir / std::sqrt(dir.x * dir.x + dir.y * dir.y);
	}
	position_ += Game::delta_time.asSeconds() * speed_ * dir;
	sprite_.setPosition(position_);
}


}