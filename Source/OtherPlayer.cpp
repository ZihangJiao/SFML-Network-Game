#include "OtherPlayer.h"


OtherPlayer::OtherPlayer(int id, sf::Vector2f position, std::string name) : m_health(100), m_id(id), m_name("default")
{
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Media/Stay.png");
	this->setTexture(playerTexture);
	this->setPosition(position);
	this->setName(name);
}


OtherPlayer::~OtherPlayer()
{
}

