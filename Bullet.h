#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#define PI 3.14159265

#ifndef TRIUNGHI_NERVOS_BULLET_H
#define TRIUNGHI_NERVOS_BULLET_H


class Bullet
{
public:
    Bullet();
    ~Bullet();

    void init(const std::string& textureName, sf::Vector2f position, float speed,
              sf::Vector2<int> mousePos);
    void update(float dt);
    sf::Sprite getSprite();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f direction;
    float angle;
    float speed;
};


#endif
