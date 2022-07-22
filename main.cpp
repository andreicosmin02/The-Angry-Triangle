#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Triangle.h"

// Render Setup
sf::Vector2f viewSize(1600, 900);
sf::VideoMode vm((unsigned int)viewSize.x,
                 (unsigned int)viewSize.y);
sf::RenderWindow window(vm, "Triunghi Nervos",
                        sf::Style::Titlebar | sf::Style::Close);

// Textures and Sprites
sf::Texture bgTexture;
sf::Sprite bgSprite;

Triangle triangle;


void init();
void updateInput();
void update(float);
void draw();



// +++++++++++ MAIN FUNCTION +++++++++++
int main()
{
    sf::Clock clock;
    window.setFramerateLimit(60);

    init();

    while (window.isOpen())
    {
        // update input
        updateInput();

        // update game
        sf::Time dt = clock.restart();
        update(dt.asSeconds());

        // draw game
        draw();

        window.display();
    }

    return 0;
}


void init()
{
    // Create Sprites and Attach Textures
    bgTexture.loadFromFile("Assets/graphics/background.png");
    bgSprite.setTexture(bgTexture);

    triangle.init("Assets/graphics/triangle.png",
                  sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
}

void updateInput()
{
    sf::Event event{};

    while (window.pollEvent((event)))
    {
        if (event.type == sf::Event::Closed
        || event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
    }
}

void update(float dt)
{
    triangle.rotate(sf::Vector2f(sf::Mouse::getPosition(window)),
                    sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
}

void draw()
{
    window.clear(sf::Color::Blue);
    window.draw(bgSprite);
    window.draw(triangle.getSprite());
}