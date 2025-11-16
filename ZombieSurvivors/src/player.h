#pragma once
#include "texture_map.h"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace rr {
struct MoveIntent {
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
};

class Player {
public:
	sf::Vector2f position = sf::Vector2f(0.0f, 0.0f);
	MoveIntent move_intent;
private:
	float speed_ = 200.0f;
	sf::Sprite sprite_ = sf::Sprite(TextureMap::singleton()["graphics/player.png"]);

public:
	Player();
	void update(const sf::Vector2f& mouse_position);
	sf::Sprite get_visual() const { return sprite_; }
private:
	void handle_movement();
	void handle_rotation(const sf::Vector2f& mouse_position);
};
} // End namespace rr