#pragma once
#include "texture_map.h"
#include "box_collider.h"
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
	float fire_rate{1.0f};
	sf::Time last_fire_time{};
	std::unique_ptr<BoxCollider> collider_{};

public:
	void awake();
	void start(const sf::RenderWindow* window);
	void update();
	void render(sf::RenderWindow* window);
private:
	void handle_movement();
	void handle_fire(const sf::Vector2f& mouse_position);
	void look_at(const sf::Vector2f& mouse_position);
	void fire(const sf::Vector2f& dir);
};
} // End namespace rr