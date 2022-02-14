#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include<sstream>

class BackGround 
{
public:
	BackGround ();
	~BackGround ();
	int run();

private:

	void update();
	int procesEvents();
	void render();


	sf::RenderWindow mWindow;

	//BackGround
	sf::Texture background;
	sf::Sprite backgroundSprite;
	sf::Font backgroundFont;
	sf::Text backText;
};



