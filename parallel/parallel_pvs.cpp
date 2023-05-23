#include "AIPlayer.h"

#include <algorithm>
#include <omp.h>

void AIPlayer::parallel_pvs(GameBoard & gameBoard, int depth, Node & node, int alpha, int beta) {
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
	std::sort(subnodes.begin(), subnodes.end());
	int size = subnodes.size();
	if (!size) {
		return;
	}
	int n = std::min(10, size);
	if(node.type == type) { // MIN
		node.value = INT_MAX;
		// PVS split
		gameBoard.play(subnodes[0].type, subnodes[0].move);
		parallel_pvs(gameBoard, depth - 1, subnodes[0], alpha, beta);
		gameBoard.undo(subnodes[0].move);
		if((node.value > subnodes[0].value)){
			node.value = subnodes[0].value;
			node.nextMove = subnodes[0].move;
		}
		beta = std::min(beta, node.value);
		if(beta < alpha){
			return;
		}
		// other branch
		#pragma omp parallel
		{
			GameBoard localBoard(gameBoard);
			#pragma omp for schedule(static)
			for(int i = 1; i < n; ++i) {
				localBoard.play(subnodes[i].type, subnodes[i].move);
				sequential_alphabeta(localBoard, depth - 1, subnodes[i], beta - 1, beta);
				if (subnodes[i].value > alpha && subnodes[i].value < beta) {
					sequential_alphabeta(localBoard, depth - 1, subnodes[i], alpha, beta);
				}
				// traceback
				localBoard.undo(subnodes[i].move);
				// update the node value
				if((node.value > subnodes[i].value)){
					node.value = subnodes[i].value;
					node.nextMove = subnodes[i].move;
				}
				beta = std::min(beta, node.value);
			}
		}
	}
	else {  //MAX
		node.value = INT_MIN;
		// PVS split
		gameBoard.play(subnodes[size - 1].type, subnodes[size - 1].move);
		sequential_pvs(gameBoard, depth - 1, subnodes[size - 1], alpha, beta);
		gameBoard.undo(subnodes[size - 1].move);
		// update the node value
		if((node.value < subnodes[size - 1].value)){
			node.value = subnodes[size - 1].value;
			node.nextMove = subnodes[size - 1].move;
		}
		alpha = std::max(alpha, node.value);
		if(alpha > beta){
			return;
		}
		// other branch
		#pragma omp parallel
		{
			GameBoard localBoard(gameBoard);
			#pragma omp for schedule(static)
			for(int i = 2; i <= n; ++i) {
				localBoard.play(subnodes[size - i].type, subnodes[size - i].move);
				sequential_alphabeta(localBoard, depth - 1, subnodes[size - i], alpha, alpha + 1);
				if (subnodes[size - i].value > alpha && subnodes[size - i].value < beta) {
					sequential_alphabeta(localBoard, depth - 1, subnodes[size - i], alpha, beta);
				}
				// traceback
				localBoard.undo(subnodes[size - i].move);
				// update the node value
				if((node.value < subnodes[size - i].value)){
					node.value = subnodes[size - i].value;
					node.nextMove = subnodes[size - i].move;
				}
				alpha = std::max(alpha, node.value);
			}
		}
	}
}