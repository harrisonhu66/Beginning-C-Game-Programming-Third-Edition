#pragma once
#include "player.h"
#include <SFML/System/Time.hpp>

namespace rr {
class Zombie {
private:
	Player* player_ = nullptr;
	// TODO This is stupid, always need to code the stats of some actor
	sf::Vector2f position_ = sf::Vector2f(0, 0);
	float speed_ = 0.0f;
	sf::Sprite sprite_ = sf::Sprite();

public:
	enum class Type {
		crawler,
		chaser,
		bloater
	};
	constexpr static int num_types = 3;

	Zombie(Type type);
	void set_position(float x, float y) {
		position_ = sf::Vector2f(x, y);
	}
	void set_chase_target(Player* player) {
		player_ = player;
	}

	void update();

	sf::Sprite get_visual() const {
		return sprite_;
	}
private:
	void chase_target();
};
}
