#ifndef _GAME_BOARD_H_
#define _GAME_BOARD_H_

#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <algorithm>

#define BOARD_SIZE 15

#define POSITION(x, y) ((x) * BOARD_SIZE + (y)) 
#define XPOS(pos) ((pos) / BOARD_SIZE)
#define YPOS(pos) ((pos) % BOARD_SIZE)
#define IS_SAME_TYPE(chess, type) (chess != 0 && ((type == BC && chess > 0) || (type == WC && chess < 0)))
#define OPP(type) (ChessType(-type))
#define IN_BOARD(xpos, ypos) ((xpos) >= 0 && (xpos) < BOARD_SIZE && (ypos) >= 0 && (ypos) < BOARD_SIZE)

enum ChessType{
	ZERO = 0, BC = 1, WC = -1
};

typedef std::array<int, 5> FiveTuple;
enum ShapeType{
	T_NO = 0, T_ONE, T_TWO, T_STHR, T_LTHR, T_CFOUR, T_LFOUR, T_FIVE
};

struct BoardUnit{
	int pos;
	std::vector<int> tuple_index;
};

class GameBoard {
public: 
	GameBoard() { board.fill(0); };

	GameBoard(const GameBoard & gameBoard): board(gameBoard.board) {};

	bool is_valid_move(int pos) {
		if (pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE && 0 == board[pos]) {
			return true;
		}
		return false;
	}

	std::vector<int> available_moves(int step = 1) {
		std::vector<int> moves;
		for(int i = 0, n = BOARD_SIZE * BOARD_SIZE; i < n; ++i) {
			if (has_neighbor(i, step) && 0 == board[i]) {
				moves.push_back(i);
			}
		}
		return moves;
	}
	
	void play(ChessType type, int pos) {
		assert(WC == type || BC == type);
		assert(pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE);
		assert(0 == board[pos]);
		board[pos] = type;
	}
	
	void undo(int pos) {
		assert(pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE);
		assert(0 != board[pos]);
		board[pos] = 0;
	}

	void reset() {
		board.fill(0);
	}

	int eval_global(ChessType type) {
		assert(type == WC || type == BC);
		int value = 0;
		for(auto & [_, tuple] : GameBoard::fiveTuples){
			value += eval_tuple(type, tuple);
		}
		return value;
	}

	int eval_pos(ChessType type, int pos) {
		assert(type == WC || type == BC);
		assert(pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE);
		int value = 0;
		for(auto & index : GameBoard::boardUnits[pos].tuple_index) {
			value += eval_tuple(type, GameBoard::fiveTuples[index]);
		}
		return value;
	}

	bool win(ChessType type, int pos) {
		assert(BC == type || WC == type);
		assert(pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE);
		auto judge = [type, this](int p) {
			return type == board[p];
		}; 
		for(int index : boardUnits[pos].tuple_index){
			if(5 == std::count_if(fiveTuples[index].begin(), fiveTuples[index].end(), judge)){
				return true;
			}
		}
		return false;
	}

	void print() {
		std::cout<<std::setw(3)<<" ";
		for(int i = 0; i < BOARD_SIZE; ++i){
			std::cout<< std::setw(3)<<i;
		}
		std::cout<<std::endl;
		for(int i = 0; i < BOARD_SIZE; ++i){
			std::cout<<std::setw(3)<<i;
			for(int j = 0; j < BOARD_SIZE; ++j){
				if (BC == board[POSITION(i, j)]){
					std::cout<<std::setw(3)<<"O";
				} else if (WC == board[POSITION(i, j)]){
					std::cout<<std::setw(3)<<"X";
				}else{
					std::cout<<std::setw(3)<<".";
				}
			}
			std::cout<<std::endl;
		}
	}

	static inline std::vector<BoardUnit> boardUnits;
	static inline std::unordered_map<int, FiveTuple> fiveTuples;
	static inline std::array<int, 8> shape_score{0, 1, 20, 500, 1200, 4000, 100000, 1000000};
	
	static void init_fiveTuples() {
		GameBoard::boardUnits.resize(BOARD_SIZE * BOARD_SIZE);
		FiveTuple tuple;
		auto fiveTuple_index = [] (const FiveTuple & fiveTuple) {
			return (fiveTuple[0] << 8) + fiveTuple[4];
		};
		for(int pos = 0, size = BOARD_SIZE * BOARD_SIZE; pos < size; ++pos){
			GameBoard::boardUnits[pos].pos = pos;
			int xpos = XPOS(pos), ypos = YPOS(pos);
			for(int i = 4; i >= 0; --i){
				if(xpos - i >= 0 && xpos - i + 4 < BOARD_SIZE){
					for(int j = 0; j < 5; ++j){
						tuple[j] = POSITION(xpos - i + j, ypos);
					}
					int index = fiveTuple_index(tuple);
					GameBoard::boardUnits[pos].tuple_index.push_back(index);
					if(0 == fiveTuples.count(index)){
						GameBoard::fiveTuples.emplace(index, tuple);
					}
				}
				if(ypos - i >= 0 && ypos - i + 4 < BOARD_SIZE){
					for(int j = 0; j < 5; ++j){
						tuple[j] = POSITION(xpos, ypos - i + j);
					}
					int index = fiveTuple_index(tuple);
					GameBoard::boardUnits[pos].tuple_index.push_back(index);
					if(0 == fiveTuples.count(index)){
						GameBoard::fiveTuples.emplace(index, tuple);
					}
				}
				if(xpos - i >= 0 && ypos - i >= 0 && xpos - i + 4 < BOARD_SIZE && ypos - i + 4 < BOARD_SIZE){
					for(int j = 0; j < 5; ++j){
						tuple[j] = POSITION(xpos - i + j, ypos - i + j);
					}
					int index = fiveTuple_index(tuple);
					GameBoard::boardUnits[pos].tuple_index.push_back(index);
					if(0 == fiveTuples.count(index)){
						GameBoard::fiveTuples.emplace(index, tuple);
					}
				}
				if(xpos - i >= 0 && ypos + i < BOARD_SIZE && xpos - i + 4 < BOARD_SIZE && ypos + i - 4 >= 0){
					for(int j = 0; j < 5; ++j){
						tuple[j] = POSITION(xpos - i + j, ypos + i - j);
					}
					int index = fiveTuple_index(tuple);
					GameBoard::boardUnits[pos].tuple_index.push_back(index);
					if(0 == fiveTuples.count(index)){
						GameBoard::fiveTuples.emplace(index, tuple);
					}
				}
			}
		}
	}

private:
	std::array<int, BOARD_SIZE * BOARD_SIZE> board;

	bool has_neighbor(int pos, int step) {
		assert(pos >= 0 && pos < BOARD_SIZE * BOARD_SIZE);
		int xpos = XPOS(pos), ypos = YPOS(pos);
		int left = std::max(xpos - step, 0), 
			right = std::min(xpos + step, BOARD_SIZE - 1),
			up = std::max(ypos - step, 0),
			down = std::min(ypos + step, BOARD_SIZE - 1);
		for(int x = left; x <= right; ++x){
			for(int y = up; y<= down; ++y){
				if((x != xpos || y != ypos) && 0 != board[POSITION(x, y)]) {
					return true;
				}
			}
		}
		return false;
	}

	ShapeType tuple_shape_type(ChessType type, const FiveTuple & tuple) {
		int n = 0;
		for(int i = 0; i < 5; ++i){
			if(type == board[tuple[i]]){
				++n;
			}else if(OPP(type) == board[tuple[i]]){
				n = -1;  
				break;
			}
		}
		ShapeType shapeType;
		int dx = 0;
		int dy = 0;
		switch(n) {
		case -1:
		case 0:
			shapeType = T_NO;
			break;
		case 1: 
			shapeType = T_ONE;
			break;
		case 2: 
			shapeType = T_TWO;
			break;
		case 3: 
			if(0 == board[tuple[0]] 
				&& 0 == board[tuple[4]]){
				shapeType = T_LTHR;
			}else{
				shapeType = T_STHR;
			}
			break;
		case 4: 
			dx =  (XPOS(tuple[4]) - XPOS(tuple[0])) / 5;
			dy = (YPOS(tuple[4]) - YPOS(tuple[0])) / 5;
			if((IN_BOARD(XPOS(tuple[0]) - dx, YPOS(tuple[0]) - dy) 
				&& 0 == board[tuple[4]]) 
				|| (IN_BOARD(XPOS(tuple[4]) + dx, YPOS(tuple[4]) + dy) 
				&& 0 == board[tuple[0]])) {
					shapeType = T_LFOUR;
				}else{
					shapeType = T_CFOUR;
				}
			break;
		case 5: 
			shapeType = T_FIVE;
			break;
		default: 
			assert(0);
		}
		return shapeType;
	}

	int eval_tuple(ChessType type, const FiveTuple & tuple) {
		ShapeType shapeType = tuple_shape_type(type, tuple);
		return shape_score[shapeType];
	}
};	

#endif