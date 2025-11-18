#include "bullet.h"
#include "game.h"

namespace rr {

void Bullet::awake() {
    constexpr int texture_size = 4;
    shape_.setSize(sf::Vector2f(texture_size, texture_size));
    shape_.setOrigin(texture_size / 2.0f, texture_size / 2.0f);

	auto sprite_size = shape_.getGlobalBounds().getSize();
    collider_ = std::make_unique<BoxCollider>(sprite_size);
}

void Bullet::start(const Stats& stats) {
    stats_ = stats;
}

void Bullet::update() {
    if (!stats_.is_flying) {
        return;
    }
	fly();
	// God damn, handle-written component update is stupid
	collider_->update(stats_.position);
}

void Bullet::render(sf::RenderWindow* window) {
    if (!stats_.is_flying) {
        return;
    }
    window->draw(shape_);
    collider_->render(window);
}

const BoxCollider& Bullet::get_collider() const {
    return *collider_;
}

void Bullet::fly() {
    auto velocity = stats_.speed * stats_.dir;
	stats_.position += Game::delta_time.asSeconds() * velocity;
    shape_.setPosition(stats_.position);
    auto pixel = stats_.window->mapCoordsToPixel(shape_.getGlobalBounds().getPosition());
	auto window_size = stats_.window->getSize();
    auto boundary = sf::Vector2i(window_size.x, window_size.y);
	bool out_of_bounds = pixel.x < 0 || pixel.y < 0 || pixel.x > boundary.x || pixel.y > boundary.y ;
    stats_.is_flying = velocity != sf::Vector2f(0, 0) && !out_of_bounds;
}

}
