#include "game.h"

int main(int argc, char const *argv[])
{
	GameBoard::init_fiveTuples();
	Game game;
	game.ready(Game::HumanvsAI);
	game.play();
	return 0;
}