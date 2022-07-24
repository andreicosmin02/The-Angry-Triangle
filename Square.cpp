#include "Square.h"

#include <cmath>

Square::Square()
= default;

Square::~Square()
= default;

void Square::init(std::string textureName, sf::Vector2f position, sf::Vector2f targetPos)
{
    texture.loadFromFile(textureName.c_str());
    sprite.setTexture(texture);

    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2,
                     texture.getSize().y / 2);


    sf::Vector2f direction(targetPos.x - sprite.getPosition().x,
                           targetPos.y - sprite.getPosition().y);

    angle = atan2(direction.y, direction.x);
}

void Square::moveTowards(float speed, float dt)
{


    sf::Vector2f newPos(cos(angle) * speed * dt, sin(angle) * speed * dt);
    sprite.move(newPos.x , newPos.y );
}

sf::Sprite Square::getSprite()
{
    return sprite;
}