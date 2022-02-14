#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "OtherPlayer.h"
#include <iostream>
#include <memory>
class Network
{
public:
	Network(sf::IpAddress& ip, unsigned short& port);
	void disconnect(Player* p);

	void sendDirection(Player* p);
	void sendPosition(Player* p);
	int receive(std::vector<std::unique_ptr<Player>>& otherPlayers, Player* p); //Add more later
	void sendMessage(Player* p, std::string& text);
	void sendMyName(Player* p);
	void getPlayerList(Player* p);
	void sendBullets(Player* p);
	void sendHost_confirm();
	void send_HP(int num, float hp);

	std::string getMessage() { return m_textMessage; }

private:
	sf::TcpSocket connection;
	bool m_connected;

	std::string m_textMessage;
	int packetCounter = 0;
	sf::Clock packetClock;
};

