#pragma once
#include "player.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace rr {
class Game {
	// Due to deleted operator= of sf::Window, so choose the pointer type
	sf::Vector2f window_center_;
	sf::RenderWindow* window_;
	bool is_running_ = true;

	rr::Player player_;
public:
	Game(const std::string title);
	~Game();
	void run();
	void setup();
	void handle_input();
	void update(sf::Time dt);
	void render();
};

} // End namespace rr