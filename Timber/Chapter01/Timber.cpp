// Include important libraries here
#include <array>
#include <sstream>
#include <format>
#include <iostream>
#include <cstdlib>
#include <ctime>
// Abstract 2D graphics like sprites, texts, shapes, ...
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Make code easier to type with "using namespace" 
// This is a bad practice, just use using-declartion or qualified names
// It introduces names into the LCA scope of the target scope and current scope (which is the global scope)
// https://quuxplusone.github.io/blog/2020/12/21/using-directive/
//using namespace sf;

void update_branches();
constexpr int num_branches = 6;
std::array<sf::Sprite, num_branches> branches{};
enum class Side {
	none,
	left,
	right,
};
std::array<Side, num_branches> branch_positions{};


// This is where our game starts from int main()
// OS calls main() and receive a value indicating this program success or not.
int main() {
	sf::VideoMode video_mode{1920, 1080};
	sf::RenderWindow window{video_mode, "Timber!!!"};

	// Seed the random number generator, so you will get distinct random numbers each time the program runs
	std::srand(std::time(0));

	sf::Clock clock;

	float time_bar_width = 400, time_bar_height = 400;
	sf::RectangleShape time_bar;
	time_bar.setSize(sf::Vector2f{time_bar_width, time_bar_height});
	time_bar.setFillColor(sf::Color::Red);
	time_bar.setPosition(1920 / 2.0f - time_bar_width / 2, 980);
	float time_remaining = 6.0f;
	float time_bar_width_per_second = time_bar_width / time_remaining;

	bool paused{true}; // first run at pause state
	int score = 0;

	sf::Texture background_texture{}; 
	// load the texure into the video memory
	background_texture.loadFromFile("graphics/background.png"); 
	sf::Sprite background_sprite{background_texture};
	background_sprite.setPosition(0, 0);

	// REFACTOR 重复出现load texture from a path, create a correspoding sprite a given localtion
	// auto sprite = SpriteRegistry::Create({x, y}, <path or some identity(int32, string, ...) pointing to a specific texture>);
	// identity-address the texture, share them among spirtes.
	// If you find yourself copying and pasting a block of code, you have probably found an opportunity for functional abstraction.
	// https://www.composingprograms.com/pages/14-designing-functions.html
	sf::Texture tree_texture{};
	tree_texture.loadFromFile("graphics/tree.png");
	sf::Sprite tree_sprite{tree_texture};
	tree_sprite.setPosition(810, 0); // 1920/2=810, fuck 当时没有计算验证一下 -> 1920/2 = 960, so 960 - 810 = 150 = tree image width / 2

	sf::Texture bee_texture{};
	bee_texture.loadFromFile("graphics/bee.png");
	sf::Sprite bee_sprite{bee_texture};
	float bee_height = 800;
	bee_sprite.setPosition(0, bee_height);
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

	sf::Font font{};
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	sf::Text message_text{"Press Enter to start!", font, 75};
	sf::Text score_text{"Score = 0", font, 100};
	message_text.setFillColor(sf::Color::White);
	score_text.setFillColor(sf::Color::White);
	const auto message_rect = message_text.getLocalBounds();
	std::cout << std::format("left={}, top={}, width={}, height={}\n", message_rect.left, message_rect.top, message_rect.width, message_rect.height);
	// The entity's position is the position of its origin, 所有变换都是以origin为中心的
	// https://www.sfml-dev.org/tutorials/3.0/graphics/transform/#scale
	// 我之前理解错了, 我以为改变origin会改变局部坐标系的远点, 实际上不会, 局部坐标系原点一直是(0, 0)不会变
	// 这里的意图是setPosition设置的是文本中心到目标位置, 而不是左上角到目标位置
	// TMD, 名字起个origin原点, 我还以为改局部坐标系原点, 叫TransformationOrigin多好
	// 把它设置到局部系的某个点, 之后设置位置就是移动这个点
	// 虽然变化矩阵中向量确实以origin作为tail end, 但是这里还有一个坐标系的概念, 所以我弄混了nima
	// Ref: Chapter7 Transformation Matrices <<Fundamentals of Computer Graphics 5th edition>>
	message_text.setOrigin(message_rect.left + message_rect.width / 2.0f, message_rect.top + message_rect.height / 2.0f);
	std::cout << std::format("{}, {}", message_text.getOrigin().x, message_text.getOrigin().y);
	message_text.setPosition(1920 / 2.0f, 1080 / 2.0f);
	// postion score at the top left with a little bit padding
	score_text.setPosition(20, 20);

	sf::Texture branch_texture{};
	branch_texture.loadFromFile("graphics/branch.png");
	for (auto i = branches.begin(), e = branches.end(); i != e; ++i) {
		i->setTexture(branch_texture);
		i->setPosition(-2000, -2000); // just fuck off the screen
		
		// MAYBE 先看看效果再说
		// 设置中心就是可以直接旋转而不改变位置
		i->setOrigin(220, 40); 
	}

	// Player on the left of the screen on the beginning
	sf::Texture player_texture{};
	player_texture.loadFromFile("graphics/player.png");
	sf::Sprite player_sprite{player_texture};
	player_sprite.setPosition(580, 720);
	Side player_side = Side::left;

	sf::Texture gravestone_texture{};
	// annoying, make sth called TextureStore.get("rip" /* <name/id> */)
	gravestone_texture.loadFromFile("graphics/rip.png");
	sf::Sprite gravestone_sprite{gravestone_texture};
	gravestone_sprite.setPosition(600, 860);

	sf::Texture axe_texture{};
	axe_texture.loadFromFile("graphics/axe.png");
	sf::Sprite axe_sprite{axe_texture};
	axe_sprite.setPosition(700, 830);

	constexpr float axe_position_left{700};
	constexpr float axe_position_right{1075};

	sf::Texture log_texture{};
	log_texture.loadFromFile("graphics/log.png");
	sf::Sprite log_sprite{log_texture};
	log_sprite.setPosition(810, 720);
	bool log_active{false};
	float log_speed_x{1000};
	float log_speed_y{-1500};

	bool accept_input{false};

	sf::SoundBuffer chop_buffer{};
	chop_buffer.loadFromFile("sound/chop.wav");
	sf::Sound chop{};
	chop.setBuffer( chop_buffer );

	sf::SoundBuffer death_buffer{};
	death_buffer.loadFromFile("sound/death.wav");
	sf::Sound death{};
	death.setBuffer( death_buffer );

	sf::SoundBuffer oot_buffer{};
	oot_buffer.loadFromFile("sound/out_of_time.wav");
	sf::Sound oot{};
	oot.setBuffer( oot_buffer );

	float score_update_timer = 0;
	float score_update_duration = 0.1; // Normally, human relection is 100ms max

	while (window.isOpen()) {
		//
		// Input
		//
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyReleased && !paused) {
				accept_input = true;

				axe_sprite.setPosition(2000, axe_sprite.getPosition().y);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			// Fuck, add a spdlog with fmt
			//std::cout << "esc\n";
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			//std::cout << "enter\n";
			paused = false;

			score = 0;
			time_remaining = 6.0f;

			for (auto& position : branch_positions) {
				position = Side::none;
			}
			// Move gravestone away
			gravestone_sprite.setPosition(675, 2000);
			// Move player into the screen, 死亡的时候会被移出视口/屏幕
			player_sprite.setPosition(580, 720);	

			accept_input = true;
		}

		if (accept_input) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				player_side = Side::right;
				++score;
				time_remaining += (2.0f / score) + 0.15f;

				axe_sprite.setPosition(axe_position_right, axe_sprite.getPosition().y);
				player_sprite.setPosition(1200, 720);

				update_branches();
				
				log_sprite.setPosition(810, 720);
				log_speed_x = -5000;
				log_active = true;

				accept_input = false;

				chop.play();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				player_side = Side::left;
				++score;
				time_remaining += (2.0f / score) + 0.15f;

				axe_sprite.setPosition(axe_position_left, axe_sprite.getPosition().y);
				player_sprite.setPosition(580, 720);

				update_branches();

				// 如果按太快, 木头飞一半就消失了
				// https://store.steampowered.com/app/398710/Timberman/ 是有多个圆木飞出
				log_sprite.setPosition(810, 720);
				log_speed_x = 5000;
				log_active = true;

				accept_input = false;

				chop.play();
			}
		}
		// End Input
		//

		//
		// Update
		//

		if (!paused) {
			sf::Time dt = clock.restart();

			static float t = 0;
			t += dt.asSeconds();

			time_remaining -= dt.asSeconds();
			time_bar.setSize(sf::Vector2f{time_remaining*time_bar_width_per_second, time_bar_height});

			if (time_remaining <= 0.0f) {
				// MAYBE goto rendering pipeline
				// 其实也还好, 还会更新一下当前帧, 但是我觉得直接跳转更好
				paused = true;

				message_text.setString("Out of time!!");

				auto message_rect = message_text.getLocalBounds();
				message_text.setOrigin(message_rect.left + message_rect.width / 2, message_rect.top + message_rect.height / 2);
				message_text.setPosition(1920 / 2.0f, 1080 / 2.0f);

				oot.play();
			}

			// Bee-respawning
			if (!bee_active) {
				//bee_speed = (rand() % 200) + 200; // [200, 200+200=400)
				// Make a static function, Random::GetInt(/*left*/ 0, /*right_exclusively*/ 10) //[0, 10)
				bee_speed = (rand() % 400) + 200; // [200, 200+200=400)

				bee_height = (rand() % 500) + 500;
				bee_sprite.setPosition(2000, bee_height); // 从最右边向左边移动

				bee_active = true;
			}
			else {
				float distance = dt.asSeconds() * bee_speed;
				// y = Asin(wt+fi)+D 天然上下波动
				//float frequency = (2 * std::acos(-1)) / 0.7f; // omiga = 2*pi / t t一个周期
				float frequency = (2 * std::acos(-1)) / 0.7f; // omiga = 2*pi / t t一个周期
				float siny = std::sin(t * frequency);

				float amplitude = 30.f;
				float y = amplitude * siny + bee_height;

				//bee_sprite.setPosition(bee_sprite.getPosition().x - distance, bee_sprite.getPosition().y);
				bee_sprite.setPosition(bee_sprite.getPosition().x - distance, y);

				// -100 is a hack
				if (bee_sprite.getPosition().x < -100) { // off-screen to the left
					bee_active = false;
				}
			}

			// Clouds moving left to right
			if (!cloud_active1) {
				cloud_speed1 = rand() % 200 + 10; // 如果是0, 云就不会respawn
				float height = rand() % 150;
				cloud_sprite1.setPosition(-200, height);
				cloud_active1 = true;
			}
			else {
				float distance = dt.asSeconds() * cloud_speed1;
				cloud_sprite1.setPosition(cloud_sprite1.getPosition().x + distance, cloud_sprite1.getPosition().y);

				// 这里还有一个问题, 1920 is a magic number
				if (cloud_sprite1.getPosition().x > 1920) { // top-left corner is the origin
					cloud_active1 = false;
				}
			}

			if (!cloud_active2) {
				cloud_speed2 = rand() % 200 + 20;
				float height = rand() % 300 - 150; // negative bound, weird
				cloud_sprite2.setPosition(-200, height);
				cloud_active2 = true;
			}
			else {
				float distance = dt.asSeconds() * cloud_speed2;
				cloud_sprite2.setPosition(cloud_sprite2.getPosition().x + distance, cloud_sprite2.getPosition().y);

				if (cloud_sprite2.getPosition().x > 1920) {
					cloud_active2 = false;
				}
			}

			if (!cloud_active3) {
				cloud_speed3 = rand() % 200 + 30;
				float height = rand() % 450 - 150;
				cloud_sprite3.setPosition(-200, height);
				cloud_active3 = true;
			}
			else {
				float distance = dt.asSeconds() * cloud_speed3;
				cloud_sprite3.setPosition(cloud_sprite3.getPosition().x + distance, cloud_sprite3.getPosition().y);

				if (cloud_sprite3.getPosition().y > 1920) {
					cloud_active3 = false;
				}
			}

			score_update_timer += dt.asSeconds();
			if (score_update_timer >= score_update_duration) {
				// Update the score text
				std::stringstream ss{};
				ss << "Score = " << score;
				//std::format("Score = {}", score); 用于字符的拼接方便多了, stream系列的API设计挺差的
				score_text.setString(ss.str());
				score_update_timer = 0;
			}

			for (int i{0}, e{static_cast<int>(branches.size())}; i < e; ++i) {
				float height{i * 150.0f};
				switch (branch_positions[i]) {
				using enum Side;
				case left:
					// 810-220 = 590 <-> +20 :可以更嵌入树干吗?
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
					break;
				case right:
					// treeLeft + treeWidth + branchWidth/2 = 1330
					// 810 + 300 + 220 = 1330
					branches[i].setPosition(1330, height);
					// 如果这个 Sprite 在之前被旋转过（比如从左切换到右），不设 setRotation(0)，它仍可能保持旧的 180° 朝向。
					branches[i].setRotation(0);
					break;
				default:
					branches[i].setPosition(3000, height);
					break;
				}
			}
			if (log_active)
			{
				log_sprite.setPosition(
					log_sprite.getPosition().x +
					(log_speed_x * dt.asSeconds()),

					log_sprite.getPosition().y +
					(log_speed_y * dt.asSeconds()));
				// Has the log reached the right hand edge?
				if (log_sprite.getPosition().x < -100 ||
					log_sprite.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					log_active = false;
					log_sprite.setPosition(810, 720);
				}
			}

			if (branch_positions[5] == player_side) {
				paused = true;
				accept_input = false;

				gravestone_sprite.setPosition(525, 760);

				player_sprite.setPosition(2000, 660);

				message_text.setString("SQUISHED!");
				auto rect = message_text.getLocalBounds();
				message_text.setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);
				message_text.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}
		} // End if(!paused
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
		window.draw(background_sprite); 
		window.draw(cloud_sprite1);
		window.draw(cloud_sprite2);
		window.draw(cloud_sprite3);
		for (auto i = branches.begin(), e = branches.end(); i != e; ++i) {
			window.draw(*i);
		}
		window.draw(tree_sprite);

		window.draw(player_sprite);
		window.draw(axe_sprite);
		window.draw(log_sprite);
		window.draw(gravestone_sprite);

		window.draw(bee_sprite);

		// HUD-specific
		window.draw(score_text);
		window.draw(time_bar);
		if (paused) {
			window.draw(message_text);
		}

		window.display(); // Double-buffering switching/flipping
		//
		// End Draw
		//
	}
	return 0;
}

void update_branches() {
	for (int i{static_cast<int>(branch_positions.size()) - 1}; i > 0; --i) {
		branch_positions[i] = branch_positions[i - 1];
	}
	branch_positions[0] = []() {
		switch (rand() % 5) {
		case 0:
			return Side::left;
		case 1:
			return Side::right;
		default:
			return Side::none;
		}
	}();
}

