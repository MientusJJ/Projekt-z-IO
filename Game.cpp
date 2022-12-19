#include "Game.h"

Game::Game()
{
	srand(time(NULL));
}
void Game::play()
{
	hero = Hero::getInstance();
	Chamber *CurrentCham = new StartingRoom(hero);
	while (CurrentCham != nullptr)
	{
		CurrentCham = CurrentCham->takeAction(hero);
	}
}
Game::~Game()
{
	delete hero;
}