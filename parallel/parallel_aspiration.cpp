#include "AIPlayer.h"

#include <algorithm>
#include <omp.h>

void AIPlayer::parallel_aspiration(GameBoard & gameBoard, int depth, Node & node) {
	int windows[5] = {INT_MIN, -10000, 0, 10000, INT_MAX}; // split the alpha-beta window
	std::vector<Node> tmp(4, node);
	#pragma omp parallel
	{
		GameBoard localBoard(gameBoard);
		#pragma omp for schedule(static)
		for(int i = 0; i < 4; ++i) {
			sequential_alphabeta(localBoard, depth, tmp[i], windows[i], windows[i+1]);
		}
	}
	auto max_it = std::max_element(tmp.begin(), tmp.end());
	node.value = max_it->value;
	node.nextMove = max_it->nextMove;
}