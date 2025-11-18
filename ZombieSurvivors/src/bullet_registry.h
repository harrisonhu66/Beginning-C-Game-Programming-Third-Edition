#pragma once
#include "bullet.h"
#include <SFML/Graphics.hpp>
#include <array>

namespace rr {
class BulletRegistry {
private:
	std::array<std::unique_ptr<Bullet>, 100> bullets_{};
	int index_ = 0;

	BulletRegistry();
public:
	static BulletRegistry& singleton() {
		static BulletRegistry instance;
		return instance;
	}

	void spawn(const sf::Vector2f& start_position, const sf::Vector2f& fly_dir, const sf::RenderWindow* window);
	void update_all();
	void render_all(sf::RenderWindow* window);
};
}