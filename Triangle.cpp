#include "Triangle.h"

Triangle::Triangle()
= default;

Triangle::~Triangle()
= default;

void Triangle::init(const std::string& textureName, sf::Vector2f position)
{
    texture.loadFromFile(textureName);
    sprite.setTexture(texture);

    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y * 2 / 3);
}

void Triangle::rotate(sf::Vector2f mousePosition)
{
    sf::Vector2f position(mousePosition.x - sprite.getPosition().x,
                          mousePosition.y - sprite.getPosition().y);
    float angle = atan2(position.y, position.x) * 180 / PI + 90;

    sprite.setRotation(angle);
}

sf::Sprite Triangle::getSprite()
{
    return sprite;
}

sf::Texture Triangle::getTexture()
{
    return texture;
}