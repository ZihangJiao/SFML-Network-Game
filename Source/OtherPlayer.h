#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class OtherPlayer : public sf::Sprite
{
public:
	OtherPlayer(int id, sf::Vector2f position, std::string name);
	~OtherPlayer();

	float getHealth(){ return m_health; }
	void setHealth(float health){ m_health = health; }

	void setID(int id){ m_id = id; }
	int getID(){ return m_id; }

	void setName(std::string name){ m_name = name; }
	std::string getName(){ return m_name; }

	void setPosition(sf::Vector2f position) { m_position = position; }
	sf::Vector2f getPosition() { return m_position; }

	void setDirection(sf::Vector2f direction) { m_direction = direction; }
	sf::Vector2f getDirection(){ return m_direction; }
private:
	sf::Texture pTexture;
	float m_health;
	int m_id;
	std::string m_name;
	sf::Vector2f m_direction;
	sf::Vector2f m_position;
};

