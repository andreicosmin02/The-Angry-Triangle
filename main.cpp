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

sf::RectangleShape triangleCollider;

void init();
void updateInput();
void update(float);
void draw();

void spawnSquare();
void shoot();
bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
bool checkCollision(const sf::RectangleShape& rect, const sf::Sprite& sprite);
void reset();

float currentTime = 0.0f;
float prevTime;

int score = 0;
bool gameOver = true;

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
        if (!gameOver)
        {
            update(dt.asSeconds());
        }

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

    triangleCollider.setFillColor(sf::Color::Transparent);
    triangleCollider.setSize(sf::Vector2f(triangle.getTexture().getSize().x / 2,
                                        triangle.getTexture().getSize().y / 2));
    triangleCollider.setPosition(viewSize.x / 2, viewSize.y / 2);
    triangleCollider.setOrigin(triangleCollider.getSize().x / 2,
                             triangleCollider.getSize().y / 2);
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
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                if (gameOver)
                {
                    gameOver = false;
                    reset();
                }
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
    if (currentTime >= prevTime + 0.5)
    {
        spawnSquare();
        prevTime = currentTime;
    }
    for (Square *square : squares)
    {
        square->moveTowards(400, dt);
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

    for (int i = 0; i < squares.size(); i++)
    {
        Square* square = squares[i];
        for (int j = 0; j < bullets.size(); j++)
        {
            // Check collision between Squares and Bullets

            Bullet* bullet = bullets[j];

            if (checkCollision(bullet->getSprite(), square->getSprite()))
            {
                score++;

                squares.erase(squares.begin() + i);
                bullets.erase(bullets.begin() + j);

                delete(bullet);
                delete(square);
            }
        }

        sf::FloatRect shape1 = triangleCollider.getGlobalBounds();
        sf::FloatRect shape2 = square->getSprite().getGlobalBounds();

        if (checkCollision(triangleCollider, square->getSprite()))
        {
            gameOver = true;
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
    window.draw(triangleCollider);
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
    auto* square = new Square();
    square->init("Assets/graphics/square.png", squarePos,
                 triangle.getSprite().getPosition());
    squares.push_back(square);
}

void shoot()
{
    auto* bullet = new Bullet();

    bullet->init("Assets/graphics/bullet.png",
                 triangle.getSprite().getPosition(),
                 800.0f, sf::Mouse::getPosition(window));
    bullets.push_back(bullet);
}

bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
{
    sf::FloatRect shape1 = sprite1.getGlobalBounds();
    sf::FloatRect shape2 = sprite2.getGlobalBounds();

    if (shape1.intersects(shape2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool checkCollision(const sf::RectangleShape& rect, const sf::Sprite& sprite)
{
    sf::FloatRect shape1 = rect.getGlobalBounds();
    sf::FloatRect shape2 = sprite.getGlobalBounds();

    if (shape1.intersects(shape2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void reset()
{
    score = 0;
    currentTime = 0.0f;
    prevTime = 0.0f;

    for (Square *square : squares)
    {
        delete(square);
    }
    for (Bullet *bullet : bullets)
    {
        delete(bullet);
    }

    squares.clear();
    bullets.clear();
}