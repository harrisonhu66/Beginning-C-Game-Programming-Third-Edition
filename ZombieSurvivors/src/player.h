#pragma once
#include "texture_map.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace rr {

class Player {
public:
	sf::Vector2f position{};
private:
	float speed_{};
	sf::Sprite sprite_{};
	const sf::RenderWindow* window_{};

public:
	void awake();
	void start(const sf::RenderWindow* window);
	void update();
	const sf::Sprite& get_visual() const;
private:
	void handle_movement();
	void look_at(const sf::Vector2f& mouse_position);
};
} // End namespace rr