#pragma once
#include "arena.h"
#include "zombie.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

namespace rr {
class Game {
private:
	// Due to deleted operator= of sf::Window, so choose the pointer type
	sf::VideoMode video_mode_;
	sf::Vector2f window_center_;
	std::unique_ptr<sf::RenderWindow> window_;

	bool is_running_;

	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Zombie>> zombies_;

	sf::Vector2f mouse_position_;

	std::unique_ptr<Arena> arena_;
	sf::View main_camera_;
public:
	/**
	 * @brief Time between frames
	 */
	inline static sf::Time delta_time = sf::Time();
public:
	Game(const std::string& title);
	void loop();
	void prepare();
	void process_inputs();
	void update();
	void render();
};

} // End namespace rr