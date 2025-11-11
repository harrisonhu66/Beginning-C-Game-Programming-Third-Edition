#include <SFML/Graphics.hpp>

class Bat {
public:
    Bat(float x, float y) {
        this->shape.setPosition(x, y);
    }

    void update(sf::Time dt) {
        auto position = this->shape.getPosition();
        if (wanted_to_move_right) {
            position.x += dt.asSeconds() * this->horizontal_speed;
        }
        if (wanted_to_move_left) {
            position.x -= dt.asSeconds() * this->horizontal_speed;
        }
        this->shape.setPosition(position);
    }

    const sf::RectangleShape& get_shape() const {
        return this->shape;
    }

    bool wanted_to_move_left{};
    bool wanted_to_move_right{};
private:
    sf::RectangleShape shape{{50, 5}};
    float horizontal_speed{1000.0f};
};

int main()
{
    sf::VideoMode vm{1920, 1080};
    sf::RenderWindow window{vm, "Pong"};
    
    Bat bat{1920 / 2, 1080 - 20};

    sf::Clock clock{};
    while (window.isOpen())
    {
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
        }
        else {
            bat.wanted_to_move_left = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            bat.wanted_to_move_right = true;
        }
        else {
            bat.wanted_to_move_right = false;
        }

        sf::Time dt = clock.restart();
        bat.update(dt);

        window.clear();
        window.draw(bat.get_shape());
        window.display();
    }
    return 0;
}
