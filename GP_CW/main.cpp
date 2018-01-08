#include <iostream>
//#include "MainGame.h"
#include "SurfGame.h"

int main(int argc, char** argv) //argument used to call SDL main
{
	//MainGame* mainGame = new MainGame();
	//mainGame->run();
	//delete mainGame;
	SurfGame* surfGame = new SurfGame();
	surfGame->run();
	delete surfGame;

	return 0;
}