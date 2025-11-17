#pragma once
#include "texture_map.h"
#include <SFML/Graphics.hpp>

namespace rr {
class Arena {
private:
	//sf::IntRect region_;
	//int tile_size_;

	struct Visual {
		sf::VertexArray vertices;
		sf::Texture* texture;
	};

	Visual visual_;
public:
	/**
	 * @brief region.width和region.heigth != tile_size * z (z in positive integers)会出现黑边，或者说会少一个tile
	 */
	Arena(const sf::IntRect& region, int tile_size);
	const Visual& get_visual() const;

	// MAYBE Add a reset_region -> wave specific thing -> becomes bigger and bigger
	// void reset_region(const sf::IntRect& region);
};
}