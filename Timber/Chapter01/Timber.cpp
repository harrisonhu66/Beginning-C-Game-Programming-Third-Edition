// Include important libraries here
#include <array>
#include <sstream>
#include <format>
#include <iostream>
#include <cstdlib>
#include <ctime>
// Abstract 2D graphics like sprites, texts, shapes, ...
#include <SFML/Graphics.hpp>

// Make code easier to type with "using namespace" 
// This is a bad practice, just use using-declartion or qualified names
// It introduces names into the LCA scope of the target scope and current scope (which is the global scope)
// https://quuxplusone.github.io/blog/2020/12/21/using-directive/
//using namespace sf;

void UpdateBranches();
constexpr int NumBranches = 6;
std::array<sf::Sprite, NumBranches> branches{};
enum class Side {
	None,
	Left,
	Right,
};
std::array<Side, NumBranches> branchPositions{};

// This is where our game starts from int main()
// OS calls main() and receive a value indicating this program success or not.
int main() {
	sf::VideoMode videoMode{1920, 1080};
	sf::RenderWindow window{videoMode, "Timber!!!"};

	// Seed the random number generator, so you will get distinct random numbers each time the program runs
	std::srand(std::time(0));

	sf::Clock clock;

	float timeBarWidth = 400, timeBarHeight = 400;
	sf::RectangleShape timeBar;
	timeBar.setSize(sf::Vector2f{timeBarWidth, timeBarHeight});
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition(1920 / 2.0f - timeBarWidth / 2, 980);
	float timeRemaing = 6.0f;
	float timeBarWidthPerSecond = timeBarWidth / timeRemaing;

	bool paused{true}; // first run at pause state
	int score = 0;

	sf::Texture backgroundTexture{}; 
	// load the texure into the video memory
	backgroundTexture.loadFromFile("graphics/background.png"); 
	sf::Sprite backgroundSprite{backgroundTexture};
	backgroundSprite.setPosition(0, 0);

	// REFACTOR 重复出现load texture from a path, create a correspoding sprite a given localtion
	// auto sprite = SpriteRegistry::Create({x, y}, <path or some identity(int32, string, ...) pointing to a specific texture>);
	// identity-address the texture, share them among spirtes.
	// If you find yourself copying and pasting a block of code, you have probably found an opportunity for functional abstraction.
	// https://www.composingprograms.com/pages/14-designing-functions.html
	sf::Texture treeTexture{};
	treeTexture.loadFromFile("graphics/tree.png");
	sf::Sprite treeSprite{treeTexture};
	treeSprite.setPosition(810, 0); // 1920/2=810, fuck 当时没有计算验证一下 -> 1920/2 = 960, so 960 - 810 = 150 = tree image width / 2

	sf::Texture beeTexture{};
	beeTexture.loadFromFile("graphics/bee.png");
	sf::Sprite beeSprite{beeTexture};
	float beeHeight = 800;
	beeSprite.setPosition(0, beeHeight);
	bool beeActive{false};
	float beeSpeed{0.0f}; // pixels per second

	sf::Texture cloudTexture{};
	cloudTexture.loadFromFile("graphics/cloud.png");
	sf::Sprite cloudSprite1{cloudTexture};
	sf::Sprite cloudSprite2{cloudTexture};
	sf::Sprite cloudSprite3{cloudTexture};
	cloudSprite1.setPosition(0, 0);
	cloudSprite2.setPosition(0, 250);
	cloudSprite3.setPosition(0, 500);
	bool cloudActive1{false};
	bool cloudActive2{false};
	bool cloudActive3{false};
	float cloudSpeed1{0.0f};
	float cloudSpeed2{0.0f};
	float cloudSpeed3{0.0f};

	sf::Font font{};
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	sf::Text messageText{"Press Enter to start!", font, 75};
	sf::Text scoreText{"Score = 0", font, 100};
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);
	const auto messageRect = messageText.getLocalBounds();
	std::cout << std::format("left={}, top={}, width={}, height={}\n", messageRect.left, messageRect.top, messageRect.width, messageRect.height);
	// The entity's position is the position of its origin, 所有变换都是以origin为中心的
	// https://www.sfml-dev.org/tutorials/3.0/graphics/transform/#scale
	// 我之前理解错了, 我以为改变origin会改变局部坐标系的远点, 实际上不会, 局部坐标系原点一直是(0, 0)不会变
	// 这里的意图是setPosition设置的是文本中心到目标位置, 而不是左上角到目标位置
	// TMD, 名字起个origin原点, 我还以为改局部坐标系原点, 叫TransformationOrigin多好
	// 把它设置到局部系的某个点, 之后设置位置就是移动这个点
	// 虽然变化矩阵中向量确实以origin作为tail end, 但是这里还有一个坐标系的概念, 所以我弄混了nima
	// Ref: Chapter7 Transformation Matrices <<Fundamentals of Computer Graphics 5th edition>>
	messageText.setOrigin(messageRect.left + messageRect.width / 2.0f, messageRect.top + messageRect.height / 2.0f);
	std::cout << std::format("{}, {}", messageText.getOrigin().x, messageText.getOrigin().y);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	// postion score at the top left with a little bit padding
	scoreText.setPosition(20, 20);

	sf::Texture branchTexture;
	branchTexture.loadFromFile("graphics/branch.png");
	for (auto i = branches.begin(), e = branches.end(); i != e; ++i) {
		i->setTexture(branchTexture);
		i->setPosition(-2000, -2000); // just fuck off the screen
		
		// MAYBE 先看看效果再说
		// 设置中心就是可以直接旋转而不改变位置
		i->setOrigin(220, 20); // 左边上移20也行
	}

	for (int i{0}; i < 5; ++i) {
		UpdateBranches();
	}

	while (window.isOpen()) {
		//
		// Input
		//
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			// Fuck, add a spdlog with fmt
			//std::cout << "esc\n";
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			//std::cout << "enter\n";
			paused = false;

			score = 0;
			timeRemaing = 6.0f;
		}
		//
		// End Input
		//

		//
		// Update
		//

		if (!paused) {
			sf::Time dt = clock.restart();

			static float t = 0;
			t += dt.asSeconds();

			timeRemaing -= dt.asSeconds();
			timeBar.setSize(sf::Vector2f{timeRemaing*timeBarWidthPerSecond, timeBarHeight});

			if (timeRemaing <= 0.0f) {
				// MAYBE goto rendering pipeline
				// 其实也还好, 还会更新一下当前帧, 但是我觉得直接跳转更好
				paused = true;

				messageText.setString("Out of time!!");

				auto messageRect = messageText.getLocalBounds();
				messageText.setOrigin(messageRect.left + messageRect.width / 2, messageRect.top + messageRect.height / 2);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// Bee-respawning
			if (!beeActive) {
				//bee_speed = (rand() % 200) + 200; // [200, 200+200=400)
				// Make a static function, Random::GetInt(/*left*/ 0, /*right_exclusively*/ 10) //[0, 10)
				beeSpeed = (rand() % 400) + 200; // [200, 200+200=400)

				beeHeight = (rand() % 500) + 500;
				beeSprite.setPosition(2000, beeHeight); // 从最右边向左边移动

				beeActive = true;
			}
			else {
				float distance = dt.asSeconds() * beeSpeed;
				// y = Asin(wt+fi)+D 天然上下波动
				//float frequency = (2 * std::acos(-1)) / 0.7f; // omiga = 2*pi / t t一个周期
				float frequency = (2 * std::acos(-1)) / 0.7f; // omiga = 2*pi / t t一个周期
				float siny = std::sin(t * frequency);

				float amplitude = 30.f;
				float y = amplitude * siny + beeHeight;

				//bee_sprite.setPosition(bee_sprite.getPosition().x - distance, bee_sprite.getPosition().y);
				beeSprite.setPosition(beeSprite.getPosition().x - distance, y);

				// -100 is a hack
				if (beeSprite.getPosition().x < -100) { // off-screen to the left
					beeActive = false;
				}
			}

			// Clouds moving left to right
			if (!cloudActive1) {
				cloudSpeed1 = rand() % 200 + 10; // 如果是0, 云就不会respawn
				float height = rand() % 150;
				cloudSprite1.setPosition(-200, height);
				cloudActive1 = true;
			}
			else {
				float distance = dt.asSeconds() * cloudSpeed1;
				cloudSprite1.setPosition(cloudSprite1.getPosition().x + distance, cloudSprite1.getPosition().y);

				// 这里还有一个问题, 1920 is a magic number
				if (cloudSprite1.getPosition().x > 1920) { // top-left corner is the origin
					cloudActive1 = false;
				}
			}

			if (!cloudActive2) {
				cloudSpeed2 = rand() % 200 + 20;
				float height = rand() % 300 - 150; // negative bound, weird
				cloudSprite2.setPosition(-200, height);
				cloudActive2 = true;
			}
			else {
				float distance = dt.asSeconds() * cloudSpeed2;
				cloudSprite2.setPosition(cloudSprite2.getPosition().x + distance, cloudSprite2.getPosition().y);

				if (cloudSprite2.getPosition().x > 1920) {
					cloudActive2 = false;
				}
			}

			if (!cloudActive3) {
				cloudSpeed3 = rand() % 200 + 30;
				float height = rand() % 450 - 150;
				cloudSprite3.setPosition(-200, height);
				cloudActive3 = true;
			}
			else {
				float distance = dt.asSeconds() * cloudSpeed3;
				cloudSprite3.setPosition(cloudSprite3.getPosition().x + distance, cloudSprite3.getPosition().y);

				if (cloudSprite3.getPosition().y > 1920) {
					cloudActive3 = false;
				}
			}

			// Update the score text
			std::stringstream ss{};
			ss << "Score = " << score;
			//std::format("Score = {}", score); 用于字符的拼接方便多了, stream系列的API设计挺差的
			scoreText.setString(ss.str());

			for (int i{0}, e{static_cast<int>(branches.size())}; i < e; ++i) {
				float height{i * 150.0f};
				switch (branchPositions[i]) {
				using enum Side;
				case Left:
					// 810-220 = 590 <-> +20 :可以更嵌入树干吗?
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
					break;
				case Right:
					// treeLeft + treeWidth + branchWidth/2 = 1330
					// 810 + 300 + 220 = 1330
					branches[i].setPosition(1330, height);
					break;
				default:
					branches[i].setPosition(3000, height);
					break;
				}
			}
		} // End if(!paused)
		//
		// End Update
		//

		//
		// Draw
		//
		window.clear();

		// Painter's algorithms indicates that the order of drawing is important
		// https://en.wikipedia.org/wiki/Painter%27s_algorithm

		// 这里的GPU流水线应该还有更多的信息, 但是先抓住重点
		// put the texture to some display buffer (e.g. framebuffer)
		window.draw(backgroundSprite); 
		window.draw(cloudSprite1);
		window.draw(cloudSprite2);
		window.draw(cloudSprite3);
		for (auto i = branches.begin(), e = branches.end(); i != e; ++i) {
			window.draw(*i);
		}
		window.draw(treeSprite);
		window.draw(beeSprite);

		// HUD-specific
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused) {
			window.draw(messageText);
		}

		window.display(); // Double-buffering switching/flipping
		//
		// End Draw
		//
	}
	return 0;
}

void UpdateBranches() {
	for (int i{static_cast<int>(branchPositions.size()) - 1}; i > 0; --i) {
		branchPositions[i] = branchPositions[i - 1];
	}
	branchPositions[0] = []() {
		switch (rand() % 5) {
		case 0:
			return Side::Left;
		case 1:
			return Side::Right;
		default:
			return Side::None;
		}
	}();
}
