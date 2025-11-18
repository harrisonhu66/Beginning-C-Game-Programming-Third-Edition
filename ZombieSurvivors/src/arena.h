#pragma once
#include "texture_map.h"
#include <SFML/Graphics.hpp>

namespace rr {
class Arena {
private:
	sf::IntRect region_{};
	int tile_size_{};

	sf::VertexArray vertices{};
	sf::Texture* texture{};
public:
	/**
	 * @brief region.width和region.heigth != tile_size * z (z in positive integers)会出现黑边，或者说会少一个tile
	 */
	Arena(const sf::IntRect& region, int tile_size);
	void render(sf::RenderWindow* window);
	// MAYBE Add a reset_region -> wave specific thing -> becomes bigger and bigger
	// void reset_region(const sf::IntRect& region);

	const sf::IntRect& get_region() const { return region_; }
	int get_tile_size() const { return tile_size_; }
};
} // End namespace rr