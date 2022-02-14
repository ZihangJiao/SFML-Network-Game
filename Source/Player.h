#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.h"
class Player : public sf::Sprite
{
public:

	//Player(int id, sf::Vector2f position, std::string name);
	Player();
	~Player();
	void setHealth(float health){ m_health = health; }
	float getHealth(){ return m_health; }

	void setID(int id){ m_id = id; m_isReady = true; }
	int getID(){ return m_id; }

	bool isReady(){ return m_isReady; }
	float getAttackDamage(){ return m_attackDamage; }

	void setName(std::string name) { m_name = name; }
	std::string getName(){ return m_name; }

	//void setPosition(sf::Vector2f position) { m_position = position; }
	//sf::Vector2f getPosition() { return m_position; }

	void setDirection(sf::Vector2f direction) { m_direction = direction; }
	sf::Vector2f getDirection(){ return m_direction; }

	void setFaceDirection(sf::Vector2f facedirection) { m_face_direction = facedirection; }
	sf::Vector2f getFaceDirection() { return m_face_direction; }

	void setBulletList(std::vector<Bullet> bullets) {
		m_bullets = bullets;
	}
	std::vector<Bullet> getBulletList() {
		return m_bullets;
	}

	void setPressShoot(bool event) {
		press_shoot = event;
	}
	bool getPressShoot() {
		return press_shoot;
	}

	void setBulletTimer(float timer) {
		the_bullet_timer = timer;
	}
	float getBulletTimer() {
		return the_bullet_timer;
	}

	void setBulletShoot(bool event) {
		bullet_shoot = event;
	}
	bool getBulletShoot() {
		return bullet_shoot;
	}

	void setHost(bool is_host_in) {
		is_host = is_host_in;
	}
	bool getHost() {
		return is_host;
	}

	void setHP(float hp_in) {
		hp = hp_in;
	}
	float getHP() {
		return hp;
	}

	void setInitialised(bool initilised) {
		m_initialised = initilised;
	}
	float getInitialised() {
		return m_initialised;
	}

	void setDead(bool is_dead) {
		dead = is_dead;
	}
	float getDead() {
		return dead;
	}


private:
	// Player sprite and texture
	sf::Texture pTexture;
	float m_health;
	int m_id;
	bool m_isReady;
	float m_attackDamage;
	std::string face_to;
	std::string m_name;
	sf::Vector2f m_position;
	sf::Vector2f m_face_direction;

	sf::Vector2f m_direction;
	std::vector<Bullet> m_bullets;
	bool press_shoot;
	float the_bullet_timer;
	bool bullet_shoot=false;
	bool is_host = false;
	float hp = 100.0;

	bool m_initialised = false;
	bool dead = false;
};



//if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
//	current_player.setPressShoot(true);//player has pressed shoot
//}
//else {
//	current_player.setPressShoot(false);
//	std::cout << current_player.getBulletTimer() << std::endl;
//	bullet_timer = current_player.getBulletTimer() + 1.0f;
//	current_player.setBulletTimer(bullet_timer);
//
//}
//updateBullets(current_player);
//
//	}
//	//std::cout << current_player.getMoving() << std::endl;s
// 
//
//
//}
////Bullets;
//void Game::updateBullets(Player player) {
//	if (player.getPressShoot() == true) {
//		bullet_timer = player.getBulletTimer();
//		//std::cout << player.getBulletTimer() << std::endl;
//
//		if (bullet_timer >= bullet_timer_max) {//check wheather the bullet should be shoot
//			//-(player.getSize().y / 2.0f)
//			this->bullet.setPosition(player.getPosition().x + 20.0, player.getPosition().y + 50.0);
//			this->bullet.setDirection(player.getFaceDirection());
//			bullets.push_back(bullet);
//			player.setBulletTimer(0.f);
//			//std::cout << player.getBulletTimer() << std::endl;
//		}
