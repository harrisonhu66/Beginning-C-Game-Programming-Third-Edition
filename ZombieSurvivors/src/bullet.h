#pragma once
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
public:
	void awake();
	void start(const Stats& stats);
	void update();
	sf::FloatRect get_rect() const;
	const sf::RectangleShape& get_visual() const;
	bool is_flying() const;
private:
	void fly();
};
}