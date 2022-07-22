#include "Triangle.h"

Triangle::Triangle()
{

}

Triangle::~Triangle()
{

}

void Triangle::init(std::string textureName, sf::Vector2f position)
{
    texture.loadFromFile(textureName.c_str());
    sprite.setTexture(texture);

    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y * 2 / 3);
}

void Triangle::rotate(sf::Vector2f mousePosition, sf::Vector2f screenCenter)
{
    sf::Vector2i position(mousePosition.x - screenCenter.x,
                          mousePosition.y - screenCenter.y);
    float angle = atan2(position.y, position.x) * 180 / PI + 90;

    sprite.setRotation(angle);
}

sf::Sprite Triangle::getSprite()
{
    return sprite;
}
