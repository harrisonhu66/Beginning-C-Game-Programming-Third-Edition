#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <map>

namespace rr {
class TextureMap {
	std::map<std::string, sf::Texture> textures_;
public:
	// delete all ctor, operator=
	static TextureMap& get() {
		static TextureMap singleton;
		return singleton;
	}
	sf::Texture& operator[](const std::string& filename);
};
} // End namespace rr