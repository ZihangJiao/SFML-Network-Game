#include "NetworkPlayer.h"

NetworkPlayer::NetworkPlayer(std::unique_ptr<sf::TcpSocket>* socket, sf::Vector2f position, int id)
{
	m_position = position;
	m_name = "Default Name";
	m_id = id;
	m_socket = std::move(*socket);
	m_velocity = sf::Vector2f(0, 0);
	m_timeout = sf::seconds(0);
}


void NetworkPlayer::setPosition(sf::Vector2f position)
{
	m_position = position;
}

void NetworkPlayer::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

sf::Vector2f NetworkPlayer::getPosition()
{
	return m_position;
}

void NetworkPlayer::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

void NetworkPlayer::setVelocity(float x, float y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

sf::Vector2f NetworkPlayer::getVelocity()
{
	return m_velocity;
}

void NetworkPlayer::setName(const std::string name)
{
	m_name = name;
}

std::string NetworkPlayer::getName()
{
	return m_name;
}

int NetworkPlayer::getId()
{
	return m_id;
}

sf::TcpSocket* NetworkPlayer::getSocket()
{
	return m_socket.get();
}

void NetworkPlayer::setTimeout(sf::Time time)
{
	m_timeout = time;
}

sf::Time NetworkPlayer::getTimeout()
{
	return m_timeout;
}

void NetworkPlayer::update(float deltaTime)
{
	m_position.x += (m_velocity.x * deltaTime);
	m_position.y += (m_velocity.y * deltaTime);

}

