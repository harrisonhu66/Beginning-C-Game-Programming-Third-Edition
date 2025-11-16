#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <map>

namespace rr {
class TextureMap {
	std::map<std::string, sf::Texture> textures_;
public:
	// delete all ctor, operator=
	static TextureMap& singleton() {
		static TextureMap tm;
		return tm;
	}
	sf::Texture& operator[](const std::string& filename);
};
} // End namespace rr