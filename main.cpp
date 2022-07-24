#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "Triangle.h"
#include "Square.h"
#include "Bullet.h"


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
std::vector<Bullet*> bullets;

void init();
void updateInput();
void update(float);
void draw();

void spawnSquare();
void shoot();

float currentTime = 0.0f;
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
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                shoot();
            }
        }

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

    // Update enemies
    if (currentTime >= prevTime + 1)
    {
        spawnSquare();
        prevTime = currentTime;
    }
    for (Square *square : squares)
    {
        square->moveTowards(50, dt);
    }

    // Update bullets
    for (int i = 0; i < bullets.size(); i++)
    {
        Bullet* bullet = bullets[i];

        bullet->update(dt);

        if (bullet->getSprite().getPosition().x < 0 ||
                bullet->getSprite().getPosition().x > viewSize.x ||
                bullet->getSprite().getPosition().y < 0 ||
                bullet->getSprite().getPosition().y > viewSize.y)
        {
            bullets.erase(bullets.begin() + i);
            delete(bullet);
        }
    }
}

void draw()
{
    window.clear(sf::Color::Blue);
    window.draw(bgSprite);
    for (Bullet *bullet : bullets)
    {
        window.draw(bullet->getSprite());
    }
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
            squarePos = sf::Vector2f(0, (float)randY);
            break;
        case 1:
            squarePos = sf::Vector2f((float)viewSize.x , (float)randY);
            break;
        case 2:
            squarePos = sf::Vector2f((float)randX, 0);
            break;
        case 3:
            squarePos = sf::Vector2f((float)randX, (float)viewSize.y);
            break;
        default:
            return;
    }
    Square* square = new Square();
    square->init("Assets/graphics/square.png", squarePos,
                 triangle.getSprite().getPosition());
    squares.push_back(square);
}

void shoot()
{
    Bullet* bullet = new Bullet();

    bullet->init("Assets/graphics/bullet.png", triangle.getSprite().getPosition(),
                 400.0f, sf::Mouse::getPosition(window));
    bullets.push_back(bullet);
}