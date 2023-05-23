#ifndef _HUMAN_PLAYER_H_
#define _HUMAN_PLAYER_H_

#include "gameBoard.h"
#include "gamePlayer.h"

#include <iostream>

class HumanPlayer: public GamePlayer {
public:
	HumanPlayer(ChessType type): GamePlayer(type) {}

	int play_next_move(GameBoard & gameBoard, int pre_move) {
		std::cout << "opponent in position (" << XPOS(pre_move) << ", " << YPOS(pre_move) << ")" << std::endl;
		std::cout << "available moves: ";
		std::vector<int> moves = gameBoard.available_moves();
		for(auto & i : moves) {
			std::cout << "(" << XPOS(i) << ", " << YPOS(i) << ") ";
		}
		std::cout << "\n";
		int x, y;
		do {
			std::cout << "input a valid move (row, col): ";
			std::cin >> x >> y;
		} while (! gameBoard.is_valid_move(POSITION(x, y)));
		return POSITION(x, y);
	}
}; 

#endif