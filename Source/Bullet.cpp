#include "Bullet.h";

Bullet::Bullet() {
	sf::Texture bulletTexture;
	bulletTexture.loadFromFile("Media/bubble1.png");
	this->setTexture(bulletTexture);
}
Bullet::~Bullet() {

}
