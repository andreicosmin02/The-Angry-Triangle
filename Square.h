#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#define PI 3.14159265

#ifndef TRIUNGHI_NERVOS_SQUARE_H
#define TRIUNGHI_NERVOS_SQUARE_H


class Square
{
public:
    Square();
    ~Square();

    void init(const std::string& textureName, sf::Vector2f position, sf::Vector2f targetPos);
    void moveTowards(float speed, float dt);
    sf::Sprite getSprite();
    sf::Texture getTexture();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    float angle;
};


#endif
