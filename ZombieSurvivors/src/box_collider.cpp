#include "box_collider.h"
#include "game.h"

namespace rr {

BoxCollider::BoxCollider(const sf::Vector2f& size) {
	auto center = sf::Vector2f(size.x / 2.0f, size.y / 2.0f);
	shape_.setOrigin(center);
	shape_.setSize(size);
	shape_.setFillColor(sf::Color::Transparent);
	shape_.setOutlineThickness(1.0f);
	shape_.setOutlineColor(sf::Color::Green);
}

bool BoxCollider::overlap(const BoxCollider& other) const {
	return shape_.getGlobalBounds().intersects(other.shape_.getGlobalBounds());
}

void BoxCollider::update(const sf::Vector2f& follow_position) {
	position_ = follow_position;
	shape_.setPosition(follow_position);
}

void BoxCollider::render(sf::RenderWindow* window) {
	if (Game::is_debug) {
		window->draw(shape_);
	}
}
}
