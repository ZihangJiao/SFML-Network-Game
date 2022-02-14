#include "Game.h"

const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
{
	current_player.setID(0);
	/*current_player = Player(0, sf::Vector2f(0,0),"name");*/
	m_positionTimer = sf::Time::Zero;
	port = 45000;
	ipAdress = "localhost";

	network = std::make_unique<Network>(ipAdress, port);
	
	current_player.setName("Morty");

	mWindow.create(sf::VideoMode(840, 680), "Game");
	socket.setBlocking(false);
	mWindow.setFramerateLimit(60);

	initialised = false;
	int initilise_position_confirm_counter = 0;
	//used to confirm the position is initialised

	int animation_counter = 0;

	bullet_timer_max = 10.f;
	bullet_timer = bullet_timer_max;
	current_player.setBulletTimer(bullet_timer_max);
	current_player.setHP(100.0);
	initialiseText();
	hitten = false;

	this->background.loadFromFile("Media/wood.jpg");
	backgroundSprite.setTexture(background);
	//dir[0] = 0;
	//dir[1] = 0;
	//dir[2] = 0;
	//dir[3] = 0;

	//srand(time(0));


}

Game::~Game()
{
	network->disconnect(&current_player);
}
void Game::run()
{
	network->sendHost_confirm();
	//TODO fix timestep
	while (mWindow.isOpen())
	{
		procesEvents();
		update();
		render();
	}
}



void Game::procesEvents()
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

	//TODO Organize input, Speed times delta time
	if (mWindow.hasFocus())
	{	
		if(current_player.getDead() == false){
		//Up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !movingDiagonal)
		{
			boudary_move(sf::Vector2f(0, -2), &current_player);
			current_player.setDirection(sf::Vector2f(0, -2));
			current_player.setFaceDirection(sf::Vector2f(0, -2));

		}
		//Left
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !movingDiagonal)
		{
			boudary_move(sf::Vector2f(-2, 0), &current_player);
			current_player.setDirection(sf::Vector2f(-2, 0));
			current_player.setFaceDirection(sf::Vector2f(-2, 0));
		}
		//Down
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !movingDiagonal)
		{
			boudary_move(sf::Vector2f(0, 2), &current_player);
			current_player.setDirection(sf::Vector2f(0, 2));
			current_player.setFaceDirection(sf::Vector2f(0, 2));
		}
		//Right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !movingDiagonal)
		{
			boudary_move(sf::Vector2f(2, 0), &current_player);
			current_player.setDirection(sf::Vector2f(2, 0));
			current_player.setFaceDirection(sf::Vector2f(2, 0));
		}


		//Up Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			movingDiagonal = true;

			boudary_move(sf::Vector2f(1.4, -1.4), &current_player);
			current_player.setDirection(sf::Vector2f(1.4, -1.4));
			current_player.setFaceDirection(sf::Vector2f(1.4, -1.4));
		}
		//Up Left
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			boudary_move(sf::Vector2f(-1.4, -1.4), &current_player);
			movingDiagonal = true;
			current_player.setDirection(sf::Vector2f(-1.4, -1.4));
			current_player.setFaceDirection(sf::Vector2f(-1.4, -1.4));
		}
		//Down Right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			boudary_move(sf::Vector2f(1.4, 1.4), &current_player);
			movingDiagonal = true;
			current_player.setDirection(sf::Vector2f(1.4, 1.4));
			current_player.setFaceDirection(sf::Vector2f(1.4, 1.4));
		}
		//Down Left
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			boudary_move(sf::Vector2f(-1.4, 1.4), &current_player);
			movingDiagonal = true;

			current_player.setDirection(sf::Vector2f(-1.4, 1.4));
			current_player.setFaceDirection(sf::Vector2f(-1.4, 1.4));
		}
		else
			movingDiagonal = false;

		//IF no button is pressed
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			current_player.setDirection(sf::Vector2f(0, 0));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
			if (bullet_timer >= bullet_timer_max) {
				//-(player.getSize().y / 2.0f)
				summonBullets(&current_player);
				bullet_timer = 0.f;
			}
			else {
				bullet_timer += 1.0f;
			}
		}
		}
		else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {//resurrect
			current_player.setHP(100.0);
			current_player.setDead(false);
			network->send_HP(current_player.getID(), current_player.getHP());
			}
		}


	}
	//std::cout << current_player.getMoving() << std::endl;


}


void Game::boudary_move(sf::Vector2f move_direction, Player* player) {
	if (move_direction.x > 0 && move_direction.y > 0) {
		if (mWindow.getSize().x - 85 <= player->getPosition().x &&
			player->getPosition().x <= mWindow.getSize().x - 83 &&
			mWindow.getSize().y - 116.5 <= player->getPosition().y &&
			player->getPosition().y < mWindow.getSize().y - 114.5) {
			player->move(-85 + mWindow.getSize().x - player->getPosition().x,
				-116.5 + mWindow.getSize().y - player->getPosition().y);
		}
		else if (mWindow.getSize().x - 85 <= player->getPosition().x &&
			player->getPosition().x <= mWindow.getSize().x - 83) {
			player->move(-85 + mWindow.getSize().x - player->getPosition().x, 1.4);
		}
		else if (mWindow.getSize().y - 116.5 <= player->getPosition().y &&
			player->getPosition().y < mWindow.getSize().y - 114.5) {
			player->move(1.4, -116.5 + mWindow.getSize().y - player->getPosition().y);
		}
		else {
			player->move(1.4, 1.4);
		}
	}
	else if (move_direction.x > 0 && move_direction.y < 0) {
		if (-2 <= player->getPosition().y &&
			player->getPosition().y < -0 &&
			mWindow.getSize().x - 85 <= player->getPosition().x &&
			player->getPosition().x <= mWindow.getSize().x - 83) {
			player->move(-85 + mWindow.getSize().x - player->getPosition().x,
				-2 - (player->getPosition().y));
		}
		else if (-2 <= player->getPosition().y &&
			player->getPosition().y < 0) {
			player->move(2, -2 - (player->getPosition().y));
		}
		else if (mWindow.getSize().x - 85 <= player->getPosition().x &&
			player->getPosition().x <= mWindow.getSize().x - 83) {
			player->move(-85 + mWindow.getSize().x - player->getPosition().x, -2);
		}
		else {
			player->move(1.4, -1.4);
		
	}
	}
	else if (move_direction.x < 0 && move_direction.y < 0) {
		if (-2 <= player->getPosition().y &&
			player->getPosition().y < 0 &&
			0 <= player->getPosition().x &&
			player->getPosition().x < 2) {
			player->move(-player->getPosition().x, -2 - (player->getPosition().y));
		}
		else if (-2 <= player->getPosition().y && player->getPosition().y < 0) {
			player->move(-1.4, -2 - (player->getPosition().y));
		}

		else if (0 <= player->getPosition().x && player->getPosition().x < 2) {
			player->move(-player->getPosition().x, -1.4);
		}
		else {
			player->move(-1.4, -1.4);
		}
	}
	else if (move_direction.x < 0 && move_direction.y > 0) {
		if (mWindow.getSize().y - 116.5 <= player->getPosition().y &&
			player->getPosition().y < mWindow.getSize().y - 114.5 &&
			0 <= player->getPosition().x &&
			player->getPosition().x < 2) {
			player->move(-player->getPosition().x,
				-85 + mWindow.getSize().y - player->getPosition().y);
		}
		else if (mWindow.getSize().y - 116.5 <= player->getPosition().y
			&& player->getPosition().y < mWindow.getSize().y - 114.5) {
			player->move(-1.4, -116.5 + mWindow.getSize().y - player->getPosition().y);
		}
		else if (0 <= player->getPosition().x && player->getPosition().x < 2) {
			player->move(-player->getPosition().x, 1.4);
		}
		else {
			player->move(-1.4, 1.4);
		}
	}
	else if (move_direction.x > 0) {
		if (mWindow.getSize().x - 85 <= player->getPosition().x && player->getPosition().x <= mWindow.getSize().x - 83) {
			player->move(-85 + mWindow.getSize().x - player->getPosition().x, 0);
		}
		else {
			player->move(2, 0);

		}
	}
	else if (move_direction.x < 0) {
		if (0 <= player->getPosition().x && player->getPosition().x < 2) {
			player->move(-player->getPosition().x, 0);
		}
		else {
			player->move(-2, 0);
		}
	}
	else if (move_direction.y > 0) {
		if (mWindow.getSize().y - 116.5 <= player->getPosition().y && player->getPosition().y < mWindow.getSize().y - 114.5) {
			player->move(0, -116.5 + mWindow.getSize().y - player->getPosition().y);
		}
		else {
			player->move(0, 2);
		}
	}
	else if (move_direction.y < 0) {
		if (-2 <= player->getPosition().y && player->getPosition().y < 0) {
			player->move(0, -2 - (player->getPosition().y));
		}
		else {
			player->move(0, -2);
		}
	}
}

//Bullets;
void Game::summonBullets(Player* player) {

	if (player->getFaceDirection().x < 0) {
		this->bullet.setPosition(player->getPosition().x - 50.0, player->getPosition().y + player->getGlobalBounds().height / 2);
	}
	else if (player->getFaceDirection().x > 0) {
		this->bullet.setPosition(player->getPosition().x + player->getGlobalBounds().width, player->getPosition().y + player->getGlobalBounds().height / 2);
	}
	else if (player->getFaceDirection().y < 0) {
		this->bullet.setPosition(player->getPosition().x + player->getGlobalBounds().width / 2 - 20.0, player->getPosition().y - 50.0);
	}
	else if (player->getFaceDirection().y > 0) {
		this->bullet.setPosition(player->getPosition().x + player->getGlobalBounds().width / 2 - 20.0, player->getPosition().y + player->getGlobalBounds().height);
	}


	this->bullet.setDirection(player->getFaceDirection());
	bullets.push_back(bullet);
	player->setBulletShoot(true);
}

void Game::updateBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getDirection().x < 0) {
			bullets[i].move(-10.f, 0);
		}
		else if (bullets[i].getDirection().x > 0) {
			bullets[i].move(10.f, 0);
		}
		else if (bullets[i].getDirection().y < 0) {
			bullets[i].move(0, -10.f);
		}
		else {
			bullets[i].move(0, 10.f);
		}

		if (this->bullets[i].getPosition().y < 0 || this->bullets[i].getPosition().y > mWindow.getSize().y
			|| this->bullets[i].getPosition().x < 0 || this->bullets[i].getPosition().x>mWindow.getSize().x) {
			bullets.erase(this->bullets.begin() + i);// earse the bullets outside the screen
		}
		for (int i = 0; i < otherPlayers.size(); i++) {
			for (int j = 0; j < bullets.size(); j++) {
				if (otherPlayers[i]->getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
					bullets.erase(this->bullets.begin() + j);
					float temp_hp = 0.0;
					if (otherPlayers[i]->getHP() > 25) {
						temp_hp = otherPlayers[i]->getHP() - 25.0;
					}
					else {
						temp_hp = 0.0;
						otherPlayers[i]->setDead(true);
					}
					
					otherPlayers[i]->setHP(temp_hp);
					hitten = true;
					hitten_players[otherPlayers[i]->getID()] = otherPlayers[i]->getHP();

				}
			}
		}

		for (int j = 0; j < bullets.size(); j++) {
			if (current_player.getGlobalBounds().intersects(bullets[j].getGlobalBounds())) {
				bullets.erase(this->bullets.begin() + j);
				float temp_hp = 0.0;
				if (current_player.getHP() > 25) {
					temp_hp = current_player.getHP() - 25.0;
				}
				else {
					temp_hp = 0.0;
					current_player.setDead(true);
				}
				current_player.setHP(temp_hp);
				hitten = true;
				hitten_players[current_player.getID()] = current_player.getHP();
			}
		}

	}
}


void Game::update()
{
	updateBullets();
	updateText();
	//Recive data from network
	int result = network->receive(otherPlayers, &current_player);
	/*std::cout << result << std::endl;*/
	if (result == 7 ) {

		network->sendDirection(&current_player);

		if (current_player.getHost() == true) {
			for (unsigned int i = 0; i < otherPlayers.size(); i++) {
				network->send_HP(otherPlayers[i]->getID(), otherPlayers[i]->getHP());
			}
			network->send_HP(current_player.getID(), current_player.getHP());
		}

		if (current_player.getInitialised() != true) {
			initialise_position(&current_player);
			current_player.setInitialised(true);
		}



	}
	if (result == 7 || result == 4) {
		for (unsigned int i = 0; i < otherPlayers.size(); i++) {
			if (otherPlayers[i]->getInitialised() == false) {
				initialise_position(&*otherPlayers[i]);
				std::cout << otherPlayers[i]->getInitialised() << std::endl;
			}
		}
	}


	if (current_player.getHost() == true) {
		if (hitten == true) {
			for (unsigned int i = 0; i < otherPlayers.size(); i++) {
				network->send_HP(otherPlayers[i]->getID(), otherPlayers[i]->getHP());
			}
			network->send_HP(current_player.getID(), current_player.getHP());
			hitten = false;
		}
	}

	for (unsigned int i = 0; i < otherPlayers.size(); i++)
	{
		boudary_move(otherPlayers[i]->getDirection(), &*otherPlayers[i]);
		//otherPlayers[i]->move(otherPlayers[i]->getDirection());
		updateBullets();
		if (otherPlayers[i]->getBulletShoot() == true) {
			summonBullets(&*otherPlayers[i]);
			otherPlayers[i]->setBulletShoot(false);
		}
	}


	//Send direction
	if (lastDirSent != current_player.getDirection())
	{
		network->sendDirection(&current_player); // send the direction information of current player.
		lastDirSent = current_player.getDirection();// record the direction last sent
	}

	m_positionTimer += m_clock.restart();//count the time since last call
	if (m_positionTimer >= sf::seconds(1))//Send position 1 time every 1 second
	{
		network->sendPosition(&current_player);
		m_positionTimer = sf::Time::Zero;
	}
	//send bullet has been shoot 
	if (current_player.getBulletShoot() == true) {
		network->sendBullets(&current_player);
		current_player.setBulletShoot(false);
	}




}
void Game::initialise_position(Player* player) {
	sf::Vector2f pos;
	//std::cout << otherPlayers.size();
	switch (otherPlayers.size()) {
	case 0:

		pos.x = mWindow.getSize().x - 120.0;
		pos.y = 0;
		player->setInitialised(true);
		player->setPosition(pos);
		break;
	case 1:

		pos.x = 120;
		pos.y = 0;
		player->setInitialised(true);
		player->setPosition(pos);
		break;
	case 2:
		pos.x = mWindow.getSize().x - 120.0;
		pos.y = mWindow.getSize().y - 120.0;
		player->setInitialised(true);
		player->setPosition(pos);
		break;
	case 3:
		pos.x = 120;
		pos.y = mWindow.getSize().y - 120.0;
		player->setInitialised(true);
		player->setPosition(pos);

		break;

	default:
		pos.x = mWindow.getSize().x / 2;
		pos.y = mWindow.getSize().y / 2;
		player->setInitialised(true);
		player->setPosition(pos);

	}
}


void Game::initialiseText() {
	font.loadFromFile("Media/Dosis-Light.ttf");

	this->HP_Text.setFont(this->font);
	this->HP_Text.setCharacterSize(40);
	this->HP_Text.setFillColor(sf::Color::Black);
	this->HP_Text.setString("None");


	sf::FloatRect textRect = HP_Text.getLocalBounds();
	this->HP_Text.setOrigin(textRect.top + textRect.width / 2, textRect.top + textRect.height / 2);
	this->HP_Text.setPosition(100, 100);

}
void Game::updateText() {
	std::stringstream ss;
	if (current_player.getHost() == true && current_player.getDead() == true) {
		ss << "You are host" << "\n" <<"You are dead" << "\n" <<"Press R to resurrect";
		this->HP_Text.setString(ss.str());
	}else if (current_player.getHost() == true) {
		ss << "You are host";
		this->HP_Text.setString(ss.str());
	}else if (current_player.getDead() == true) {
		ss << "You are dead" <<"\n"<< "Press R to resurrect";
		this->HP_Text.setString(ss.str());

	}
	

	
}

void Game::renderText() {

	mWindow.draw(HP_Text);
}

void Game::renderPlayer(Player player) {


	std::string left_image = "Media/LeftStay.png", left_walk1 = "Media/LeftWalk1.png", left_walk2 = "Media/LeftWalk2.png";
	std::string right_image = "Media/RightStay.png", right_walk1 = "Media/RightWalk1.png", right_walk2 = "Media/RightWalk2.png";
	std::string up_image = "Media/UpStay.png", up_walk1 = "Media/WalkUp1.png", up_walk2 = "Media/WalkUp2.png";
	std::string down_image = "Media/Stay.png", down_walk1 = "Media/WalkDown1.png", down_walk2 = "Media/WalkDown2.png";

	if (player.getFaceDirection().x > 0) {
		loadPlctures(player, right_walk1, right_walk2, right_image);
	}
	else if (player.getFaceDirection().x < 0) {
		loadPlctures(player, left_walk1, left_walk2, left_image);
	}
	else if (player.getFaceDirection().y > 0) {
		loadPlctures(player, down_walk1, down_walk2, down_image);
	}
	else if (player.getFaceDirection().y < 0) {
		loadPlctures(player, up_walk1, up_walk2, up_image);
	}
	else if (player.getFaceDirection().x == 0 && player.getFaceDirection().y == 0) {
		playerTexture.loadFromFile(down_image);
	}
	player.setTexture(playerTexture);

	hp_bar_green.setPosition(player.getPosition().x, player.getPosition().y - 20.0);
	hp_bar_green.setSize(sf::Vector2f(150.f * player.getHP() / 100.0, 20.f));
	hp_bar_green.setScale(sf::Vector2f(0.5f, 0.5f));
	hp_bar_green.setFillColor(sf::Color::Green);

	hp_bar_red.setPosition(player.getPosition().x, player.getPosition().y- 20.0 );
	hp_bar_red.setSize(sf::Vector2f(150.f, 20.f));
	hp_bar_red.setScale(sf::Vector2f(0.5f, 0.5f));
	hp_bar_red.setFillColor(sf::Color::Red);


	mWindow.draw(player);
	if (player.getHP() != 100.0) {
		mWindow.draw(hp_bar_red);
	}

	mWindow.draw(hp_bar_green);
}

void Game::loadPlctures(Player player, std::string walk1, std::string walk2, std::string stayPic) {
	if (player.getDirection().x != 0 || player.getDirection().y != 0) {
		if (animation_counter <= 10) {
			playerTexture.loadFromFile(walk1);
		}
		else {
			playerTexture.loadFromFile(walk2);
		}
		if (animation_counter > 20) {
			animation_counter = 0;
		}
	}
	else {
		playerTexture.loadFromFile(stayPic);
	}
}

void Game::renderBullets(Bullet bullet) {
	bulletTexture.loadFromFile("Media/bubble1.png");
	bullet.setTexture(bulletTexture);
	mWindow.draw(bullet);

}



void Game::render()
{
	mWindow.clear();
	mWindow.draw(backgroundSprite);
	for (unsigned int i = 0; i < otherPlayers.size(); i++) {
		if (otherPlayers[i]->getInitialised() == true && otherPlayers[i]->getDead() == false) {
			renderPlayer(*otherPlayers[i]);
		}

		if (otherPlayers[i]->getBulletShoot() == true) {//if other players have pressed Shoot 
			//Player p =*otherPlayers[i];
			updateBullets();
			otherPlayers[i]->setBulletShoot(false);
		}
	}
	animation_counter++;
	if (current_player.getInitialised() == true && current_player.getDead() == false) {
		renderPlayer(current_player);
	}
	if (current_player.getHost() == true || current_player.getDead() == true) {
		renderText();
	}


	for (unsigned int i = 0; i < bullets.size(); i++) {
		renderBullets(bullets[i]);
	}
	mWindow.display();
}
