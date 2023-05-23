#ifndef _GAME_H_
#define _GAME_H_

#include "gameBoard.h"
#include "gamePlayer.h"
#include "humanPlayer.h"
#include "AIPlayer.h"

#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cstdio>

class Game{
public:
	enum Mode {  // the first holds the black 
		HumanvsHuman,
		HumanvsAI,
		AIvsHuman,
		AIvsAI
	};

	Game() {
		blackPlayer  = whitePlayer = NULL;
	}

	void ready(Mode m) {
		mode = m;
	}

	void play() {
		if (HumanvsHuman == mode) {
			HumanvsHuman_play();
		} else if (HumanvsAI == mode) {
			HumanvsAI_play();
		} else if (AIvsAI == mode) {
			AIvsAI_play();
		}
	}

private:
	GamePlayer * blackPlayer, * whitePlayer;
	GameBoard board;
	Mode mode;

	void HumanvsHuman_play() {
		blackPlayer = new HumanPlayer(BC);
		whitePlayer = new HumanPlayer(WC);
		int blackPlayerMove = -1, whitePlayerMove = -1;
		while(1) {
			board.print();
			blackPlayerMove = blackPlayer->play_next_move(board, whitePlayerMove);
			board.play(BC, blackPlayerMove);
			if (board.win(BC, blackPlayerMove)) {
				std::cout << "black win!" << std::endl;
				break;
			}
			system("cls");
			board.print();
			whitePlayerMove = whitePlayer->play_next_move(board, blackPlayerMove);
			board.play(WC, whitePlayerMove);
			if (board.win(WC, whitePlayerMove)) {
				std::cout << "white win!" << std::endl;
				break;
			}
			system("cls");
		}
		delete blackPlayer;
		delete whitePlayer;
		blackPlayer  = whitePlayer = NULL;
	}

	void HumanvsAI_play() {
		blackPlayer = new HumanPlayer(BC);
		whitePlayer = new AIPlayer(WC, AIPlayer::ParallelPVS);
		int blackPlayerMove = -1, whitePlayerMove = -1;
		while(1) {
			board.print();
			blackPlayerMove = blackPlayer->play_next_move(board, whitePlayerMove);
			board.play(BC, blackPlayerMove);
			if (board.win(BC, blackPlayerMove)) {
				std::cout << "black win!" << std::endl;
				break;
			}
			system("cls");
			whitePlayerMove = whitePlayer->play_next_move(board, blackPlayerMove);
			std::cout << whitePlayerMove << std::endl;
			board.play(WC, whitePlayerMove);
			if (board.win(WC, whitePlayerMove)) {
				std::cout << "white win!" << std::endl;
				break;
			}
		}
		delete blackPlayer;
		delete whitePlayer;
		blackPlayer  = whitePlayer = NULL;
	}

	void AIvsAI_play() {
		int step = 1;
		blackPlayer = new AIPlayer(BC, AIPlayer::ParallelPVS);
		whitePlayer = new AIPlayer(WC, AIPlayer::SequentialAlphaBeta);
		int blackPlayerMove = -1, whitePlayerMove = -1;
		// first step
		blackPlayerMove = blackPlayer->play_first_move(board);
		board.play(BC, blackPlayerMove);
		printf("[%d] black move: (%d, %d)\n", step++, XPOS(blackPlayerMove), YPOS(blackPlayerMove));
		while(1) {
			whitePlayerMove = whitePlayer->play_next_move(board, blackPlayerMove);
			board.play(WC, whitePlayerMove);
			printf("[%d] white move: (%d, %d)\n", step++, XPOS(whitePlayerMove), YPOS(whitePlayerMove));
			if (board.win(WC, whitePlayerMove)) {
				std::cout << "white win!" << std::endl;
				break;
			}
			if (BOARD_SIZE * BOARD_SIZE == step) {
				std::cout << "draw!" << std::endl;
				break;
			}
			blackPlayerMove = blackPlayer->play_next_move(board, whitePlayerMove);
			board.play(BC, blackPlayerMove);
			printf("[%d] black move: (%d, %d)\n", step++, XPOS(blackPlayerMove), YPOS(blackPlayerMove));
			if (board.win(BC, blackPlayerMove)) {
				std::cout << "black win!" << std::endl;
				break;
			}
			if (BOARD_SIZE * BOARD_SIZE == step) {
				std::cout << "draw!" << std::endl;
				break;
			}
		}
		delete blackPlayer;
		delete whitePlayer;
		blackPlayer  = whitePlayer = NULL;
	}
};


#endif // _GAME_H_