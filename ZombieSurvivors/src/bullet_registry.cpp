#include "bullet_registry.h"

namespace rr {

BulletRegistry::BulletRegistry() {
	for (auto& bullet : bullets_) {
		bullet = std::make_unique<Bullet>();
	}
}


void BulletRegistry::spawn(const sf::Vector2f& start_position, const sf::Vector2f& fly_dir, const sf::RenderWindow* window) {
	bullets_[index_]->awake();
	bullets_[index_]->start(Bullet::Stats{
		.position = start_position,
		.dir = fly_dir,
		.speed = 1000.0f,
		.window = window,
		.is_flying = true,
	});
	index_ = (index_ + 1) % bullets_.size();
}

void BulletRegistry::update_all() {
	for (auto& bullet : bullets_) {
		bullet->update();
	}
}
void BulletRegistry::render_all(sf::RenderWindow* window) {
	for (auto& bullet : bullets_) {
		bullet->render(window);
	}
}

}
