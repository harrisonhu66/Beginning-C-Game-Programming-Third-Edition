#include "bullet.h"
#include "game.h"

namespace rr {

void Bullet::awake() {
    constexpr int texture_size = 10;
    shape_.setSize(sf::Vector2f(texture_size, texture_size));
    shape_.setOrigin(texture_size / 2.0f, texture_size / 2.0f);
}

void Bullet::start(const Stats& stats) {
    stats_ = stats;
}

void Bullet::update() {
	fly();
}

sf::FloatRect Bullet::get_rect() const {
	// TODO 这里耦合了Sprite显示大小和碰撞盒大小，应该单独设置一个碰撞盒
    return shape_.getGlobalBounds();
}

const sf::RectangleShape& Bullet::get_visual() const {
    return shape_;
}

bool Bullet::is_flying() const {
    return stats_.is_flying;
}
void Bullet::fly() {
	// 如果dir是0向量，说明子弹不应该飞行了
    if (stats_.dir == sf::Vector2f(0,0)) {
        stats_.is_flying = false;
        return;
    }
	stats_.position += Game::delta_time.asSeconds() * stats_.speed * stats_.dir;
    shape_.setPosition(stats_.position);
    auto pixel = stats_.window->mapCoordsToPixel(get_rect().getPosition());
	auto window_size = stats_.window->getSize();
    auto boundary = sf::Vector2i(window_size.x, window_size.y);
	bool out_of_bounds = pixel.x < 0 || pixel.y < 0 || pixel.x > boundary.x || pixel.y > boundary.y ;
    stats_.is_flying = !out_of_bounds;
}

}
