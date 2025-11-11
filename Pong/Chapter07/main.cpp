#include <SFML/Graphics.hpp>

class Bat {
public:
    Bat(float x, float y) {
        this->shape.setPosition(x, y);
    }

    void update(sf::Time dt) {
        auto position = this->shape.getPosition();
        if (this->wanted_to_move_right) {
            position.x += dt.asSeconds() * this->horizontal_speed;
        }
        if (this->wanted_to_move_left) {
            position.x -= dt.asSeconds() * this->horizontal_speed;
        }
        this->shape.setPosition(position);
    }

    const sf::RectangleShape& get_shape() const {
        return this->shape;
    }

    sf::FloatRect get_rect() const {
        return this->shape.getGlobalBounds();
    }

    bool wanted_to_move_left{};
    bool wanted_to_move_right{};
private:
    sf::RectangleShape shape{sf::Vector2f{50, 5}};
    float horizontal_speed{1000.0f};
};

class Ball {
public:
    Ball(float x, float y) {
        this->shape.setPosition(x, y);
    }

    // TODO Update appears twice in Ball and Bat
    void update(sf::Time dt) {
        auto t = dt.asSeconds();
        auto p = this->shape.getPosition();
        p.x += t * this->speed * this->x_dir;
        p.y += t * this->speed * this->y_dir;
        this->shape.setPosition(p);
    }

    void rebound_horizontal() {
        this->x_dir *= -1;
    }

    void rebound_vertical() {
        this->y_dir *= -1;
    }

    /**
     * @brief When the ball hits the bottom of the screen
     */
    void respawn() {
        // NOTE No need to change y_dir, cuz always moving downward
        this->shape.setPosition(500, 0);
    }

    // TODO twice
    sf::RectangleShape get_shape() const {
        return this->shape;
    }

    // TODO twice
    sf::FloatRect get_rect() const {
        return this->shape.getGlobalBounds();
    }
private:
    // TODO shape & speed appear twice
    sf::RectangleShape shape{sf::Vector2f{10, 10}};
    float speed{300.0f};
    float x_dir{2.0f};
    float y_dir{2.0f};
};

// TODO 添加一些砖块?
int main()
{
    sf::VideoMode vm{1920, 1080};
    sf::RenderWindow window{vm, "Pong"};
    
    Bat bat{1920 / 2, 1080 - 20};
    Ball ball{1920 / 2, 0}; // top center of the screen

    int score{0}, lives{3};
    sf::Font font{};
    font.loadFromFile("fonts/DS-DIGIT.TTF");
    sf::Text hud{"", font, 45};
    hud.setFillColor(sf::Color::White);
    hud.setPosition(20, 20);

    sf::Clock clock{};
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            bat.wanted_to_move_left = true;
        } else {
            bat.wanted_to_move_left = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            bat.wanted_to_move_right = true;
        } else {
            bat.wanted_to_move_right = false;
        }

        sf::Time dt = clock.restart();
        bat.update(dt);
        ball.update(dt);
        auto content = std::format("Score = {}, lives = {}", score, lives);
        hud.setString(content);

        auto ball_rect = ball.get_rect();
        auto window_size = window.getSize();
        if (ball_rect.top > window_size.y) { // hits the bottom
            ball.respawn();
            --lives;
            if (lives < 1) {
                score = 0;
                lives = 3;
            }
        }
        if (ball_rect.top < 0) { // his the top
            ball.rebound_vertical();
            ++score;
        }
        if (ball_rect.left < 0 || ball_rect.left + ball_rect.width > window_size.x) {
            ball.rebound_horizontal();
        }

        auto bat_rect = bat.get_rect();
        if (ball_rect.intersects(bat_rect)) {
            ball.rebound_vertical();
        }

        window.clear();
        window.draw(hud);
        window.draw(bat.get_shape());
        window.draw(ball.get_shape());
        window.display();
    }
    return 0;
}
