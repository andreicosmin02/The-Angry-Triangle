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

// Audio
sf::SoundBuffer shootBuffer;
sf::SoundBuffer hitBuffer;
sf::SoundBuffer deathBuffer;

sf::Sound shootSound(shootBuffer);
sf::Sound hitSound(hitBuffer);
sf::Sound deathSound(deathBuffer);

// Text
sf::Font font;
sf::Text headingText;
sf::Text scoreText;
sf::Text tutorialText;

void init();
void updateInput();
void update(float);
void draw();

void spawnSquare();
void shoot();
bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
bool checkCollision(const sf::RectangleShape& rect, const sf::Sprite& sprite);
void reset();

unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

float clock1 = 0.0f;
float clock2 = 0.0f;
float difficulty = 5.0f;
float spawningTime = 0.5f;

int score = 0;
bool gameOver = true;
bool mute = false;

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

    // Text
    font.loadFromFile("Assets/fonts/Roboto-Bold.ttf");

    headingText.setFont(font);
    headingText.setString("THE ANGRY TRIANGLE");
    headingText.setCharacterSize(100);
    headingText.setFillColor(sf::Color::White);
    sf::FloatRect headingbounds = headingText.getLocalBounds();
    headingText.setOrigin(headingbounds.width / 2, headingbounds.height / 2);
    headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f,
                                  viewSize.x * 0.05f));

    scoreText.setFont(font);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(75);
    scoreText.setFillColor(sf::Color::White);
    sf::FloatRect scorebounds = scoreText.getLocalBounds();
    scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
    scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.05f));

    tutorialText.setFont(font);
    tutorialText.setString("Press Right Click to Start Game, Left Click to Shoot,"
                           " M to Mute");
    tutorialText.setCharacterSize(35);
    tutorialText.setFillColor(sf::Color::White);
    sf::FloatRect tutorialbounds = tutorialText.getLocalBounds();
    tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
    tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.2f));

    // Audio
    shootBuffer.loadFromFile("Assets/audio/shoot.wav");
    hitBuffer.loadFromFile("Assets/audio/hit.wav");
    deathBuffer.loadFromFile("Assets/audio/death.wav");
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
            if (event.mouseButton.button == sf::Mouse::Right && gameOver)
            {
                gameOver = false;
                reset();
            }
        }
        if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::M)
        {
            if (mute)
            {
                mute = false;
            }
            else
            {
                mute = true;
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
    unsigned long seed = mix(clock(), time(nullptr), getpid());
    srand(seed);
    clock1 += dt;
    clock2 += dt;

    triangle.rotate(sf::Vector2f(
            sf::Mouse::getPosition(window)));

    // Update enemies
    if (clock1 >= spawningTime)
    {
        spawnSquare();
        clock1 = 0;
    }
    if (clock2 >= difficulty && spawningTime >= 0.1)
    {
        spawningTime -= 0.02;
        clock2 = 0;
    }

    for (Square *square : squares)
    {
        square->moveTowards(200, dt);
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
                if (!mute)
                {
                    hitSound.play();
                }

                score++;
                std::string finalScore = "Score: " + std::to_string(score);
                scoreText.setString(finalScore);
                sf::FloatRect scorebounds = scoreText.getLocalBounds();
                scoreText.setOrigin(scorebounds.width / 2, scorebounds.height / 2);
                scoreText.setPosition(sf::Vector2f(viewSize.x * 0.5f,
                                                   viewSize.y * 0.85f));

                squares.erase(squares.begin() + i);
                bullets.erase(bullets.begin() + j);

                delete(bullet);
                delete(square);
            }
        }

        if (checkCollision(triangleCollider, square->getSprite()))
        {
            if (!mute)
            {
                deathSound.play();
            }
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

    if (gameOver)
    {
        window.draw(headingText);
        window.draw(tutorialText);
    }
    if(score > 0)
    {
        window.draw(scoreText);
    }
}

void spawnSquare()
{
    int randSide = rand() % 4;
    int randY = rand() % (int)viewSize.x;
    int randX = rand() % (int)viewSize.y;
    sf::Vector2f squarePos;

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

    if (!mute)
    {
        shootSound.play();
    }
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
    clock1 = 0.0f;
    clock2 = 0.0f;
    spawningTime = 1.0f;

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

unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}