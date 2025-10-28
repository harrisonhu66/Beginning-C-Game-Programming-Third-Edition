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

	// REFACTOR 重复出现load texture from a path, create a correspoding sprite a given localtion
	// auto sprite = SpriteRegistry::Create({x, y}, <path or some identity(int32, string, ...) pointing to a specific texture>);
	// identity-address the texture, share them among spirtes.
	sf::Texture tree_texture{};
	tree_texture.loadFromFile("graphics/tree.png");
	sf::Sprite tree_sprite{tree_texture};
	tree_sprite.setPosition(810, 0); // 1920/2=810

	sf::Texture bee_texture{};
	bee_texture.loadFromFile("graphics/bee.png");
	sf::Sprite bee_sprite{bee_texture};
	bee_sprite.setPosition(0, 800);
	bool bee_active{false};
	float bee_speed{0.0f}; // pixels per second

	sf::Texture cloud_texture{};
	cloud_texture.loadFromFile("graphics/cloud.png");
	sf::Sprite cloud_sprite1{cloud_texture};
	sf::Sprite cloud_sprite2{cloud_texture};
	sf::Sprite cloud_sprite3{cloud_texture};
	cloud_sprite1.setPosition(0, 0);
	cloud_sprite2.setPosition(0, 250);
	cloud_sprite3.setPosition(0, 500);
	bool cloud_active1{false};
	bool cloud_active2{false};
	bool cloud_active3{false};
	float cloud_speed1{0.0f};
	float cloud_speed2{0.0f};
	float cloud_speed3{0.0f};

	while (window.isOpen()) {
		// Input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		// Update

		// Draw
		window.clear();

		// Painter's algorithms indicates that the order of drawing is important
		// https://en.wikipedia.org/wiki/Painter%27s_algorithm

		// 这里的GPU流水线应该还有更多的信息, 但是先抓住重点
		// put the texture to some display buffer (e.g. framebuffer)
		window.draw(background_sprite); 
		window.draw(cloud_sprite1);
		window.draw(cloud_sprite2);
		window.draw(cloud_sprite3);
		window.draw(tree_sprite);
		window.draw(bee_sprite);

		window.display(); // Double-buffering switching/flipping
	}
	return 0;
}

