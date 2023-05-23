#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_

#include "gamePlayer.h"
#include "gameBoard.h"
#include <chrono>

#include <set>
#include <climits>

class AIPlayer: public GamePlayer {
public:
	enum Algorithm {
		SequentialMiniMax,
		ParallelMiniMax,
		SequentialAlphaBeta,
		ParallelAlphaBeta,
		SequentialPVS,
		ParallelPVS,
		ParallelAspiration
	};

	struct Node {
		ChessType type;  // if node.type is same as AI's type, the node is on the min layer 
		int move, nextMove;
		int evalue, value;

		Node(ChessType type, int move, int evalue): type(type), move(move), evalue(evalue) {};
		bool operator > (const Node & node) const {
			return evalue > node.evalue;
		}
		bool operator < (const Node & node) const {
			return evalue < node.evalue;
		}
	};

	AIPlayer(ChessType type, Algorithm algorithm): GamePlayer(type), alg(algorithm) {}

	int play_next_move(GameBoard & gameBoard, int pre_move) {
		int d = 6;
		auto start_time = std::chrono::steady_clock::now();
		Node node(OPP(type), pre_move, gameBoard.eval_global(type));
		if (SequentialMiniMax == alg) {
			sequential_minimax(gameBoard, d, node);
		}
		else if (SequentialAlphaBeta == alg) {
			sequential_alphabeta(gameBoard, d, node, INT_MIN, INT_MAX);
		}
		else if (ParallelMiniMax == alg) {
			parallel_minimax(gameBoard, d, node);
		}
		else if (ParallelAlphaBeta == alg) {
			parallel_alphabeta(gameBoard, d, node, INT_MIN, INT_MAX);
		}
		else if (SequentialPVS == alg) {
			sequential_pvs(gameBoard, d, node, INT_MIN, INT_MAX);
		}
		else if (ParallelPVS == alg) {
			parallel_pvs(gameBoard, d, node, INT_MIN, INT_MAX);
		}
		else if (ParallelAspiration == alg) {
			parallel_aspiration(gameBoard, d, node);
		}
		auto end_time = std::chrono::steady_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		std::cout <<"search "<< d <<" depth, "<< ms <<" ms consume"<<std::endl;
		return node.nextMove;
	}

private:
	Algorithm alg;

	void sequential_minimax(GameBoard & gameBoard, int depth, Node & node);
	
	void parallel_minimax(GameBoard & gameBoard, int depth, Node & node);

	void sequential_alphabeta(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta);

	void parallel_alphabeta(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta);

	void sequential_pvs(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta);

	void parallel_pvs(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta);

	void parallel_aspiration(GameBoard & gameBoard, int depth, Node & node);
};

#endif