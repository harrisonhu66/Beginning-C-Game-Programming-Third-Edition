#pragma once
#include <SFML/Graphics.hpp>

namespace rr {

// TODO 这里类似component，做一个actor-component system -> further, entity-component system
class BoxCollider {
public:
	BoxCollider(const sf::Vector2f& size);
	bool overlap(const BoxCollider& other) const;
	// 如上，可以查询到Actor的component属性，然后更新
	void update(const sf::Vector2f& follow_position);
	void render(sf::RenderWindow* window);
private:
	sf::Vector2f position_{};
	sf::RectangleShape shape_{};
};
}
