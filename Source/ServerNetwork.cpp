#include "ServerNetwork.h"


ServerNetwork::ServerNetwork()
{
	m_maxPlayerNumber = 5;
	m_currentID = 0;
	m_playerNumber = 0;

	m_isRunning = true;
	m_listener.listen(45000);
	m_selector.add(m_listener);
	std::cout << "Server is started. Waiting for clients" << std::endl;
}

void ServerNetwork::run()
{
	while (m_isRunning)
	{
		if (m_selector.wait())
		{
			if (m_selector.isReady(m_listener)) // if server is ready to receive new connections
			{
				std::unique_ptr<sf::TcpSocket> tempSocket = std::make_unique<sf::TcpSocket>();

				if (m_listener.accept(*tempSocket) == sf::Socket::Done)
				{
					if (m_playerNumber < m_maxPlayerNumber) //if server is not full
					{
						m_NetworkPlayerList.emplace_back(NetworkPlayer(&tempSocket, sf::Vector2f(0, 0), m_currentID));
						m_selector.add(*m_NetworkPlayerList.back().getSocket());
						m_playerNumber++;

						sf::Packet outPacket;
						outPacket << 0;
						outPacket << m_currentID;

						if (m_NetworkPlayerList.back().getSocket()->send(outPacket) != sf::Socket::Done) //Send client id
							std::cout << "error sending player index" << std::endl;
						m_currentID++;//add new player
					}
					else //if server is full
					{
						sf::Packet outPacket;
						outPacket << 2;
						

						if (tempSocket->send(outPacket) != sf::Socket::Done)
							std::cout << "Error sending server is full message" << std::endl;

						std::cout << "User tried to connect but the server was full" << std::endl;
					}
				}


			}
			else
			{
				//Receive data
				for (unsigned int i = 0; i < m_NetworkPlayerList.size(); i++)
				{//Receive from every player
					if (m_selector.isReady(*m_NetworkPlayerList[i].getSocket()))
					{ // if server is ready to receive from certain player
						sf::Packet received;
						if (m_NetworkPlayerList[i].getSocket()->receive(received) == sf::Socket::Done)
						{
							m_NetworkPlayerList[i].setTimeout(m_clock.getElapsedTime());

							int num, id;
							received >> num;
							received >> id;


							// 0 is sent when player establishes a connection to the server

							if (num == 1)
								// player disconnected, send message to all players to delete 
								//his character and delete him from servers players list
							{

								sendPacket(received, i);
								//send the message to all players except the disconnected player.

								std::cout << std::endl << "Client disconnected!" << std::endl;
								std::cout << "	ID: " << m_NetworkPlayerList[i].getId() << " Name: " << m_NetworkPlayerList[i].getName() << std::endl;

								m_playerNumber--;
								std::cout << "Number of players remain: " << m_playerNumber << std::endl;
								m_selector.remove(*m_NetworkPlayerList[i].getSocket());
								m_NetworkPlayerList.erase(m_NetworkPlayerList.begin() + i);
								if (i == 0 && m_NetworkPlayerList.size() != 0) {// if the host player leave the game;
									confirm_host();// the next host will be confirmed.
								}


								break;
							}
							//Number 2 means the server is full
							//it is sent only to the player who cannot connect

							else if (num == 3) //Moving direction
							{

								sendPacket(received, i);


							}
							else if (num == 4) //Position
							{

								sendPacket(received, i);

							}

							else if (num == 5) {// confirm wheather the client is the host client
								confirm_host();
							}

							else if (num == 7)
								//send client list with id and names  
								//When player recive this it goes through the list and compares it with its list, 
								//if he finds a number that he doesn't have he creates a enemy with that id
							{
								sf::Packet namePacket;
								namePacket << 7;
								namePacket << 0;
								namePacket << m_playerNumber;

								for (unsigned int j = 0; j < m_NetworkPlayerList.size(); ++j)
								{
									namePacket << m_NetworkPlayerList[j].getId();
									namePacket << m_NetworkPlayerList[j].getName();
								}

								sendPacket(namePacket);
							}
							else if (num == 8) {// bullet infotmation
								sendPacket(received, i);
							}

							else if (num == 9) {
								sendPacket(received, i);
							}

						}



						//If some player time-out-ed alert other players
						float time_difference = m_clock.getElapsedTime().asSeconds() - m_NetworkPlayerList[i].getTimeout().asSeconds();
						if (time_difference >= 5.0)
						{
							sf::Packet timeOutPacket;
							timeOutPacket << 2;
							timeOutPacket << m_NetworkPlayerList[i].getId();

							std::cout << "Player: " << m_NetworkPlayerList[i].getId() << " timeouted" << std::endl;
							sendPacket(received, i);

							m_selector.remove(*m_NetworkPlayerList[i].getSocket());
							m_NetworkPlayerList.erase(m_NetworkPlayerList.begin() + i);
							m_playerNumber--;
							break;
						}
					} // end of player socket is ready
				}
			}
		}
	}

}



void ServerNetwork::sendPacket(sf::Packet& packet, unsigned int skip)
{
	for (unsigned int i = 0; i < m_NetworkPlayerList.size(); ++i)
	{
		if (skip == i)
			continue;
		if (m_NetworkPlayerList[i].getSocket()->send(packet) != sf::Socket::Done)
		{
			std::cout << "Error sending packet in sendPacket func" << std::endl;
		}
	}
}

void ServerNetwork::confirm_host()
{
	sf::Packet namePacket;
	namePacket << 5;
	m_NetworkPlayerList[0].getSocket()->send(namePacket) != sf::Socket::Done;
}
