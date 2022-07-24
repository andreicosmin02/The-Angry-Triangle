#include "Square.h"

#include <cmath>

Square::Square()
= default;

Square::~Square()
= default;

void Square::init(std::string textureName, sf::Vector2f position)
{
    texture.loadFromFile(textureName.c_str());
    sprite.setTexture(texture);

    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2,
                     texture.getSize().y / 2);
}

void Square::moveTowards(sf::Vector2f targetPos, float speed, float dt)
{
    this->speed = speed;

    sf::Vector2f direction(targetPos.x - sprite.getPosition().x,
                           targetPos.y - sprite.getPosition().y);

    double angle = atan2(direction.y, direction.x) * 180 / PI;

    sf::Vector2f newPos(std::cos(angle) * speed * dt, std::sin(angle) * speed * dt);
    sprite.move(newPos.x , newPos.y );
}

sf::Sprite Square::getSprite()
{
    return sprite;
}