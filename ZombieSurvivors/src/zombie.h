#pragma once
#include "player.h"
#include "box_collider.h"
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
	std::unique_ptr<BoxCollider> collider_{};

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
	void render(sf::RenderWindow* window);
private:
	void chase_target();
	void set_chase_target(Player* player) {
		player_ = player;
	}
};
}
