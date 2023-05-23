#include "AIPlayer.h"

#include <algorithm>
#include <omp.h>

void AIPlayer::parallel_alphabeta(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta) {
	if (gameBoard.win(node.type, node.move) || 0 == depth) {
		node.value = node.evalue;
		return;
	}
	std::vector<int> moves = gameBoard.available_moves();
	std::vector<Node> subnodes;
	for(auto & pos : moves) {
		// calculate the diffrence in evalue at pos befor moving
		int oldPosEvaInc = gameBoard.eval_pos(type, pos) - gameBoard.eval_pos(OPP(type), pos);
		// move
		gameBoard.play(OPP(node.type), pos);
		// calculate the diffrence in evalue at pos after moving
		int newPosEvaInc = gameBoard.eval_pos(type, pos) - gameBoard.eval_pos(OPP(type), pos);
		// undo
		gameBoard.undo(pos);
		// the new global evalue  = node.evalue + newPosEvaInc - oldPosEvaInc
		subnodes.emplace_back(OPP(node.type), pos, node.evalue + newPosEvaInc - oldPosEvaInc);
	}
	std::sort(subnodes.begin(), subnodes.end(), std::greater<Node>());
	int size = subnodes.size();
	int n = std::min(10, size);
	node.value = (node.type == type ? INT_MAX : INT_MIN);
	#pragma omp parallel
	{
		GameBoard localBoard(gameBoard);
		#pragma omp for schedule(static)
		for(int i = 0; i < n; ++i){
			// make a move on the local board
			localBoard.play(subnodes[i].type, subnodes[i].move);
			sequential_alphabeta(localBoard, depth - 1, subnodes[i], alpha, beta);
			// traceback
			localBoard.undo(subnodes[i].move);
			// update the node value
			#pragma omp critical
			{
				if((node.value < subnodes[i].value)){
					node.value = subnodes[i].value;
					node.nextMove = subnodes[i].move;
				}
				alpha = std::max(alpha, node.value);
			}
		}
	}
}