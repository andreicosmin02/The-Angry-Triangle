#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

// Render Setup
sf::Vector2f viewSize(1600, 900);
sf::VideoMode vm((unsigned int)viewSize.x,
                 (unsigned int)viewSize.y);
sf::RenderWindow window(vm, "Triunghi Nervos", sf::Style::Titlebar | sf::Style::Close);

// Textures and Sprites
sf::Texture bgTexture;
sf::Sprite bgSprite;

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

}

void draw()
{
    window.clear(sf::Color::Blue);
}