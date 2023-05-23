#include "AIPlayer.h"
#include <algorithm>

void AIPlayer::sequential_minimax(GameBoard & gameBoard, int depth, Node & node) {
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
	int n = std::min(10, size);
	if(node.type == type){ // MIN
		node.value = INT_MAX;
		for(int i = 0; i < n; ++i) {
			gameBoard.play(subnodes[i].type, subnodes[i].move);
			sequential_minimax(gameBoard, depth - 1, subnodes[i]);
			// traceback
			gameBoard.undo(subnodes[i].move);
			// update the node value
			if((node.value > subnodes[i].value)){
				node.value = subnodes[i].value;
				node.nextMove = subnodes[i].move;
			}
		}
	}
	else {  //MAX
		node.value = INT_MIN;
		for(int i = 1; i <= n; ++i){
			gameBoard.play(subnodes[size - i].type, subnodes[size - i].move);
			sequential_minimax(gameBoard, depth - 1, subnodes[size - i]);
			// traceback
			gameBoard.undo(subnodes[size - i].move);
			// update the node value
			if((node.value < subnodes[size - i].value)){
				node.value = subnodes[size - i].value;
				node.nextMove = subnodes[size - i].move;
			}
		}
	}
}