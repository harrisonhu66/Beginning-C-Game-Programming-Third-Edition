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
	sf::Vector2f position_ = sf::Vector2f(0.0f, 0.0f);
	float speed_ = 200.0f;
	sf::Sprite sprite_ = sf::Sprite(rr::TextureMap::get()["graphics/player.png"]);
public:
	MoveIntent move_intent;
public:
	Player();
	void spawn(float x, float y);
	void spawn(sf::Vector2f pos) { spawn(pos.x, pos.y); }
	void update(const sf::Time& dt);
	sf::Sprite get_visual() const { return sprite_; }
};
} // End namespace rr