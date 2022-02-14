#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet : public sf::Sprite
{
public:

	Bullet();
	~Bullet();

	void setDirection(sf::Vector2f direction) { m_direction = direction; }
	sf::Vector2f getDirection() { return m_direction; }

private:
	sf::Texture pTexture;
	sf::Vector2f m_direction;
};