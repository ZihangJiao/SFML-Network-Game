#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "Network.h"
#include "ServerNetwork.h"
#include "Bullet.h"
#pragma once


class Game
{
public:
	Game();
	void run();
	~Game();
private:
	void update();
	void procesEvents();
	void render();
	void initialise_position(Player* player);
	void renderPlayer(Player player);
	void loadPlctures(Player player, std::string walk1, std::string walk2, std::string stayPic);
	void updateBullets();
	void summonBullets(Player* player);
	void renderBullets(Bullet bullet);

	void initialiseText();
	void updateText();
	void renderText();
	void boudary_move(sf::Vector2f Direction, Player* player);

	static const sf::Time timePerFrame;

	sf::RenderWindow mWindow;

	bool initialised;
	int initilise_confirm_counter;

	sf::TcpSocket socket;

	unsigned short port;
	sf::IpAddress ipAdress;

	sf::Clock m_clock;

	std::vector<sf::Sprite> remotePlayers;

	Player current_player;
	std::vector<std::unique_ptr<Player>> otherPlayers;
	std::unique_ptr<Network> network;

	sf::Vector2f lastDirSent;
	bool movingDiagonal;

	sf::Time m_positionTimer;

	sf::Texture playerTexture;
	sf::Texture bulletTexture;

	int animation_counter;
	float bullet_timer_max;
	float bullet_timer;
	Bullet bullet;
	std::vector<Bullet> bullets;
	bool sent_bullet;

	sf::Font font;
	sf::Text HP_Text;
	bool hitten = false;
	std::map<int, float> hitten_players;

	sf::RectangleShape hp_bar_green;
	sf::RectangleShape hp_bar_red;

	sf::Texture background;
	sf::Sprite backgroundSprite;

};
