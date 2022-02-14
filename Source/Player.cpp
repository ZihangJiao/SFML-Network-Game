#include "Player.h"




Player::Player() 
{	
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Media/Stay.png");
	this->setTexture(playerTexture);
}


Player::~Player()
{
}
