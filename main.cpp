#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Triangle.h"
#include "Square.h"

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
std::vector<Square*> squares;

void init();
void updateInput();
void update(float);
void draw();

void spawnSquare();

float currentTime = 100;
float prevTime;

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

    triangle.rotate(sf::Vector2f(
            sf::Mouse::getPosition(window)));

    srand((int)time(0));

    currentTime += dt;
    if (currentTime >= prevTime + 1)
    {
        spawnSquare();
        prevTime = currentTime;
    }
    for (Square *square : squares)
    {
        square->moveTowards(triangle.getPosition(), 50, dt);
    }
}

void draw()
{
    window.clear(sf::Color::Blue);
    window.draw(bgSprite);
    window.draw(triangle.getSprite());
    for (Square *square : squares)
    {
        window.draw(square->getSprite());
    }
}

void spawnSquare()
{
    int randSide = rand() % 4;
    int randY = rand() % (int)viewSize.x;
    int randX = rand() % (int)viewSize.y;
    sf::Vector2f squarePos;
    float speed;

    switch (randSide)
    {
        case 0:
            squarePos = sf::Vector2f(-100, (float)randY);
            break;
        case 1:
            squarePos = sf::Vector2f((float)viewSize.x + 100, (float)randY);
            break;
        case 2:
            squarePos = sf::Vector2f((float)randX, -100);
            break;
        case 3:
            squarePos = sf::Vector2f((float)randX, (float)viewSize.y + 100);
            break;
        default:
            return;
    }
    Square* square = new Square();
    square->init("Assets/graphics/square.png", squarePos);
    squares.push_back(square);
}