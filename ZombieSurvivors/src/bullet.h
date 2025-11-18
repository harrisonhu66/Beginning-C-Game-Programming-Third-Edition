#pragma once
#include "box_collider.h"
#include <SFML/Graphics.hpp>

namespace rr {
class Bullet {
public:
	struct Stats {
		sf::Vector2f position{};
		sf::Vector2f dir{};
		float speed{};
		const sf::RenderWindow* window{};
		bool is_flying{};
	};
private:
	Stats stats_{};

	sf::RectangleShape shape_{};
	std::unique_ptr<BoxCollider> collider_{};
public:
	void awake();
	void start(const Stats& stats);
	void update();
	void render(sf::RenderWindow* window);
	const BoxCollider& get_collider() const;
private:
	void fly();
};
}