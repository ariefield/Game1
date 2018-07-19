#include <iostream>
#include "MainGame.h"

int main(int argc, char** argv) 
{
	MainGame *game = new MainGame();
	game->run();

	return 0;
}
