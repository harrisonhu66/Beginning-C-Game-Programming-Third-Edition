#pragma once
#include "player.h"
#include <SFML/System/Time.hpp>

namespace rr {
class Zombie {
public:
	enum class Type {
		crawler,
		chaser,
		bloater
	};
	constexpr static int num_types = 3;
private:
	const Player* player_{};
	// TODO This is stupid, always need to code the stats of some actor
	sf::Vector2f position_{};
	float speed_{};
	sf::Sprite sprite_{};
	Type type_;

public:
	Zombie(Type type);

	void awake();
	void start(const Player* player);
	void set_position(float x, float y) {
		position_ = sf::Vector2f(x, y);
	}
	void prepare(Player* player) {
		set_chase_target(player);
	}

	void update();

	sf::Sprite get_visual() const {
		return sprite_;
	}
private:
	void chase_target();
	void set_chase_target(Player* player) {
		player_ = player;
	}
};
}
