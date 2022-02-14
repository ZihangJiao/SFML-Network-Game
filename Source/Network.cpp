#include "Network.h"


Network::Network(sf::IpAddress& ip, unsigned short& port) : m_connected(false)
{
	if (connection.connect(ip, port, sf::seconds(5)) != sf::Socket::Done)
	{
		std::cout << "Error connecting to server" << std::endl;
	}
	else
	{
		connection.setBlocking(false);
		std::cout << "Connected" << std::endl;
	}
}


void Network::disconnect(Player* p) //Disconnect the player from the server
{
	sf::Packet temp;
	temp << 1;
	temp << p->getID();

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending disconnect command to server" << std::endl;
	}
	else
	{
		std::cout << "Disconnected" << std::endl;
	}
}

void Network::sendDirection(Player* p)
{
	sf::Packet temp;
	temp << 3;
	temp << p->getID();
	temp << p->getDirection().x;
	temp << p->getDirection().y;
	temp << p->getFaceDirection().x;
	temp << p->getFaceDirection().y;

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending data to server" << std::endl;
	}


}

void Network::sendPosition(Player* p)
{
	if (m_connected)
	{
		sf::Packet temp;
		temp << 4;
		temp << p->getID();
		temp << p->getPosition().x;
		temp << p->getPosition().y;

		if (connection.send(temp) != sf::Socket::Done)
		{
			std::cout << "Error sending data to server" << std::endl;
		}
	}

}

void Network::sendHost_confirm()
{
	sf::Packet temp;
	temp << 5;
	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending host confirm request Information to server" << std::endl;
	}
}



void Network::sendMyName(Player* p)
{
	sf::Packet temp;
	temp << 6;
	temp << p->getID();
	temp << p->getName();

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending my name" << std::endl;
	}

}

void Network::getPlayerList(Player* p)
{
	sf::Packet temp;
	temp << 7;
	temp << p->getID();

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending getPlayerList to server" << std::endl;
	}
}


void Network::sendBullets(Player* p)
{
	sf::Packet temp;
	temp << 8;
	temp << p->getID();
	temp << p->getBulletShoot();

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending bulletInformation to server" << std::endl;
	}
}



void Network::send_HP(int id, float hp)
{
	sf::Packet temp;
	temp << 9;
	temp << id;
	temp << hp;

	if (connection.send(temp) != sf::Socket::Done)
	{
		std::cout << "Error sending HP_information to server" << std::endl;
	}
}




int Network::receive(std::vector<std::unique_ptr<Player>>& otherPlayers, Player* p)
{

	sf::Packet receivePacket;

	int type, id;

	if (connection.receive(receivePacket) == sf::Socket::Done)
	{
		receivePacket >> type;
		receivePacket >> id;
		//std::cout << type << std::endl;
		//std::cout << type << std::endl;
		if (type == 0) // you connected to server, get your ID
		{
			std::cout << p->getID() << std::endl;
			//if (p->getID() == -1)
			//{
			p->setID(id);
			std::cout << "I connected to server, my ID: " << p->getID() << std::endl;
			this->sendMyName(p);
			sf::sleep(sf::milliseconds(50));
			this->getPlayerList(p);
			//}
			m_connected = true;
			return 0;
		}
		else if (type == 1) // disconnected
		{
			for (unsigned int i = 0; i < otherPlayers.size(); i++)
			{
				if (otherPlayers[i]->getID() == id)
				{
					m_textMessage = "Player " + otherPlayers[i]->getName() + " disconnected.";
					std::cout << "OtherPlayer: " << otherPlayers[i]->getID() << " deleted " << std::endl;
					otherPlayers.erase(otherPlayers.begin() + i);
				}
			}
			return 1;
		}
		else if (type == 2)
		{
			std::cout << "Server is full" << std::endl;
			return 2;
		}

		else if (type == 3) // get move direction of enemys
		{
			for (unsigned int i = 0; i < otherPlayers.size(); i++)
			{
				if (otherPlayers[i]->getID() == id)
				{
					sf::Vector2f dir;
					receivePacket >> dir.x;
					receivePacket >> dir.y;

					otherPlayers[i]->setDirection(dir);

					sf::Vector2f dir2;
					receivePacket >> dir2.x;
					receivePacket >> dir2.y;
					otherPlayers[i]->setFaceDirection(dir2);

					break;
				}

			}
			return 3;
		}
		else if (type == 4) //Set player position to the recived position
		{

			for (unsigned int i = 0; i < otherPlayers.size(); i++)
			{
				if (otherPlayers[i]->getID() == id)
				{
					//std::cout << otherPlayers.size() << std:: endl;
					sf::Vector2f pos;
					receivePacket >> pos.x;
					receivePacket >> pos.y;
					otherPlayers[i]->setPosition(pos);
					//bool moving;
					//receivePacket >> moving;
					//std::cout << moving << std::endl;s
					break;
				}

			}
			return 4;
		}
		//Number 6 is reserved by the server for saving the name

		else if (type == 7) //Create new players
		{
			int playerNumber;
			std::vector<std::string> playersName;
			std::vector<int> playersId;

			receivePacket >> playerNumber;
			std::cout << "Num of players on server: " << playerNumber << std::endl;

			for (int i = 0; i < playerNumber; ++i)
			{
				std::string tempName;
				int tempId;
				receivePacket >> tempId;
				receivePacket >> tempName;
				playersName.push_back(tempName);
				playersId.push_back(tempId);

			}


			for (unsigned int i = 0; i < playersId.size(); ++i) //loop through id-s we got
			{
				bool haveThatEnemy = false;
				for (unsigned int v = 0; v < otherPlayers.size(); v++) //check if we already have enemy with that id
				{
					if (otherPlayers[v]->getID() == playersId[i])
					{
						haveThatEnemy = true;
					}

				}

				if (playersId[i] != p->getID() && !haveThatEnemy) //if it is not our id and if we dont have that enemy, create a new enemy with that id
				{
					Player a = Player();
					a.setID(playersId[i]);
					a.setPosition(sf::Vector2f(100, 100));
					a.setName(playersName[i]);
					
					otherPlayers.push_back(std::make_unique<Player>(a));
					//otherPlayers.push_back(std::make_unique<Player>(playersId[i], sf::Vector2f(100, 100), playersName[i]));
					m_textMessage = "New player connected: " + playersName[i];
					std::cout << "Created a new enemy with id: " << playersId[i] << std::endl;

				}
			}

			playersName.clear();
			playersId.clear();
			return 7;
		}

		else if (type == 8) {
			for (unsigned int i = 0; i < otherPlayers.size(); i++)
			{
				if (otherPlayers[i]->getID() == id)
				{
					bool Bulletshoot;
					receivePacket >> Bulletshoot;
					otherPlayers[i]->setBulletShoot(Bulletshoot);
				}

			}
			return 8;
		}
		else if (type == 5) {
			p->setHost(true);
			std::cout << "I am host" << std::endl;
			return 5;
		}
		else if (type == 9) {
			for (unsigned int i = 0; i < otherPlayers.size(); i++)
			{
				if (otherPlayers[i]->getID() == id)
				{
					float hp;
					receivePacket >> hp;
					otherPlayers[i]->setHP(hp);
					if (hp > 0) {
						otherPlayers[i]->setDead(false);
					}
				}

			}
			return 9;
		}

	}
	return -1;
}
