#pragma once
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
	sf::Vector2f window_center_;
	sf::VideoMode video_mode_;
	sf::RenderWindow* window_;
	bool running_ = true;

	Player* player_;
	Zombie* zombie_;
	std::vector<Zombie*> zombies_;
	sf::Vector2f mouse_position_;
public:
	/**
	 * @brief Time between frames
	 */
	inline static sf::Time delta_time = sf::Time();
public:
	Game(const std::string& title);
	~Game();
	void run();
	void prepare();
	void handle_input();
	void update();
	void render();
};

} // End namespace rr