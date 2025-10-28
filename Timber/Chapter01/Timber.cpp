// Include important libraries here
// Abstract 2D graphics like sprites, texts, shapes, ...
#include <SFML/Graphics.hpp>

// Make code easier to type with "using namespace" 
// This is a bad practice, just use using-declartion or qualified names
// It introduces names into the LCA scope of the target scope and current scope (which is the global scope)
// https://quuxplusone.github.io/blog/2020/12/21/using-directive/
//using namespace sf;

// This is where our game starts from int main()
// OS calls main() and receive a value indicating this program success or not.
int main() {
	sf::VideoMode video_mode{1920, 1080};
	sf::RenderWindow window{video_mode, "Timber!!!"};

	sf::Texture background_texture{}; 
	// load the texure into the video memory
	background_texture.loadFromFile("graphics/background.png"); 

	sf::Sprite background_sprite{background_texture};
	background_sprite.setPosition(0, 0);

	while (window.isOpen()) {
		// Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		// Update

		// Draw
		window.clear();

		// 这里的GPU流水线应该还有更多的信息, 但是先抓住重点
		// put the texture to some display buffer (e.g. framebuffer)
		window.draw(background_sprite); 

		window.display(); // Double-buffering switching/flipping
	}
	return 0;
}

