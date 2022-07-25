#include "Bullet.h"

Bullet::Bullet()
= default;

Bullet::~Bullet()
= default;

void Bullet::init(const std::string& textureName, sf::Vector2f position, float speed,
                  sf::Vector2<int> mousePos)
{
    this->speed = speed;
    this->position = position;

    // Load a Texture
    texture.loadFromFile(textureName);
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    // Direction
    direction = sf::Vector2f(mousePos.x - position.x,
                           mousePos.y - position.y);
    angle = atan2(direction.y, direction.x);
}

void Bullet::update(float dt)
{
    sf::Vector2f velocity(cos(angle) * speed * dt,
                          sin(angle) * speed * dt);

    sprite.move(velocity);
}

sf::Sprite Bullet::getSprite()
{
    return sprite;
}

sf::Texture Bullet::getTexture()
{
    return texture;
}