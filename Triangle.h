#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#define PI 3.14159265


#ifndef TRIUNGHI_NERVOS_TRIANGLE_H
#define TRIUNGHI_NERVOS_TRIANGLE_H


class Triangle
{
public:
    Triangle();
    ~Triangle();

    void init(const std::string& textureName, sf::Vector2f position);
    void rotate(sf::Vector2f mousePosition);
    sf::Sprite getSprite();
    sf::Texture getTexture();
private:
    sf::Texture texture;

    sf::Sprite sprite;
};


#endif
