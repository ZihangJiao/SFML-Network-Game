#include "Game.h"
#include "ServerNetwork.h"
#include "BackGround.h"
#include <thread>

void run_game() {
	Game game;
	game.run();
}

void run_server() {
	ServerNetwork server;
	server.run();
}


int main()
{
	char connectionTpe;
	bool is_server = false;

	BackGround background;
	int con_type = background.run();

	//std::cout << "Enter (s) for server, Enter (c) for client: " << std::endl;
	//std::cin >> connectionTpe;

	//if (connectionTpe == 's') {
	//	is_server = true;
	//}
	//else if (connectionTpe == 'c') {
	//}

	if (con_type == 1) {
		is_server = true;
	}
	else if (con_type == 2) {
	}

	if (is_server) {
		std::thread server(run_server);
		std::thread game(run_game);
		server.join();
		game.join();
	}
	else {
		Game game;
		game.run();
	}
	


}
