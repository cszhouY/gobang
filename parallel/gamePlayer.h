#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_

#include "gameBoard.h"

class GamePlayer {
public:
	GamePlayer(ChessType chessType): type(chessType) {}
	ChessType chess_type() {
		return type;
	}
	virtual int play_next_move(GameBoard & gameBoard, int pre_move) = 0;
	int play_first_move(GameBoard & gameBoard) {
		assert(BC == type);
		assert(gameBoard.is_valid_move(BOARD_SIZE * BOARD_SIZE / 2));
		return BOARD_SIZE * BOARD_SIZE / 2;
	}
protected:
	ChessType type;
};

#endif