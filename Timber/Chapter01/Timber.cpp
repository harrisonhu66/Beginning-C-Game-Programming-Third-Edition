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
	return 0;
}

