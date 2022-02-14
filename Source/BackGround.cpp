#include "BackGround.h"

BackGround::BackGround()
{
	mWindow.create(sf::VideoMode(840, 680), "Game");
	mWindow.setFramerateLimit(60);

	this->background.loadFromFile("Media/cosmos.jpg");
	backgroundSprite.setTexture(background);

	if (this->backgroundFont.loadFromFile("Media/font1.otf")) {
		std::cout << "load background font" << std::endl;
	}
	this->backText.setFont(this->backgroundFont);
	this->backText.setCharacterSize(50);
	this->backText.setFillColor(sf::Color::White);
	this->backText.setString("None");

	std::stringstream ss;
	ss << "Press S to be Server" << "\n" << "Press C to be Client";
	this->backText.setString(ss.str());
	sf::FloatRect textRect = backText.getLocalBounds();
	this->backText.setOrigin(textRect.top + textRect.width / 2, textRect.top + textRect.height / 2);
	this->backText.setPosition(mWindow.getView().getCenter());

}

BackGround ::~BackGround()
{
}

int BackGround ::run()
{
	while (mWindow.isOpen())
	{
		int result = procesEvents();
		if ( result == 1) {
			mWindow.close();
			return 1;
		}
		else if (result == 2) {
			mWindow.close();
			return 2;
		}
		update();
		render();
	}
	return 0;
}

int BackGround::procesEvents()
{
	//TODO organize events
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
	if (mWindow.hasFocus()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			return 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
			return 2;
		}
	}
	return 0;

}


void BackGround::update() {

}


void BackGround::render() {
	mWindow.draw(backgroundSprite);
	mWindow.draw(backText);
	mWindow.display();
}