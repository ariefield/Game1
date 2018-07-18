#include <iostream>
#include "MainGame.h"

MainGame *game = nullptr;

int main(int argc, char** argv) 
{
	game = new MainGame();
	game->run();

	return 0;
}
