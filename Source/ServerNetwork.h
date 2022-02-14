#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <memory>
#include <vector>
#include <string>
#include "NetworkPlayer.h"

class ServerNetwork
{
public:
	ServerNetwork();
	void run();

private:

	void sendPacket(sf::Packet& packet, unsigned int skip = -1);
	void confirm_host();
	bool m_isRunning;

	sf::IpAddress m_ipAdress;
	unsigned short m_port;

	sf::TcpListener m_listener;
	sf::SocketSelector m_selector;

	int m_maxPlayerNumber;
	int m_currentID;
	int m_playerNumber;

	std::vector<NetworkPlayer> m_NetworkPlayerList;
	char m_tmp[1400];

	sf::Clock m_clock;


};
