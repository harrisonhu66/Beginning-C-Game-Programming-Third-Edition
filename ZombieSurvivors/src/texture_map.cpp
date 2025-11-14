#include "texture_map.h"

namespace rr {
sf::Texture& TextureMap::operator[](const std::string& filename) {
	if (!textures_.contains(filename)) {
		auto texture = sf::Texture();
		texture.loadFromFile(filename);
		// Copy the texcture and it when it's out of scope
		// Move can eliminate this overhead
		textures_[filename] = texture; // Maybe tomorrow, use std::move
	}
	return textures_[filename];
}
} // End namespace rr