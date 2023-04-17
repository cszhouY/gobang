#include "gobang.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <climits>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <chrono>

Gobang::Gobang(ChessType player){
	this->player = player;
	computer = OPP(player);
	order = 1;
	for(int i = 0; i < BOARD_SIZE; ++i){
		for(int j = 0; j < BOARD_SIZE; ++j){
			board[i][j] = 0;
			// std::cout<<i<<" "<<j<<std::endl;
			// for(auto & tuple : boardUnits[POSITION(i, j)].get_tuples()){
			// 	for(int p : tuple){
			// 		std::cout<<"("<<XPOS(p)<<","<<YPOS(p)<<")";
			// 	}
			// 	std::cout<<std::endl;
			// }
		}
	}
	init_fiveTuples();
	if(computer == BC){
		openlist.insert(POSITION(BOARD_SIZE / 2, BOARD_SIZE / 2));
	}
}

void Gobang::init_fiveTuples(){
	boardUnits.resize(BOARD_SIZE * BOARD_SIZE);
	FiveTuple tuple;
	for(int pos = 0, size = BOARD_SIZE * BOARD_SIZE; pos < size; ++pos){
		int xpos = XPOS(pos), ypos = YPOS(pos);
		boardUnits[pos].xpos = xpos;
		boardUnits[pos].ypos = ypos;
		for(int i = 4; i >= 0; --i){
			if(xpos - i >= 0 && xpos - i + 4 < BOARD_SIZE){
				for(int j = 0; j < 5; ++j){
					tuple[j] = POSITION(xpos - i + j, ypos);
				}
				int index = fiveTuple_index(tuple);
				boardUnits[pos].tuple_index.push_back(index);
				if(0 == fiveTuples.count(index)){
					fiveTuples.emplace(index, tuple);
				}
			}
			if(ypos - i >= 0 && ypos - i + 4 < BOARD_SIZE){
				for(int j = 0; j < 5; ++j){
					tuple[j] = POSITION(xpos, ypos - i + j);
				}
				int index = fiveTuple_index(tuple);
				boardUnits[pos].tuple_index.push_back(index);
				if(0 == fiveTuples.count(index)){
					fiveTuples.emplace(index, tuple);
				}
			}
			if(xpos - i >= 0 && ypos - i >= 0 && xpos - i + 4 < BOARD_SIZE && ypos - i + 4 < BOARD_SIZE){
				for(int j = 0; j < 5; ++j){
					tuple[j] = POSITION(xpos - i + j, ypos - i + j);
				}
				int index = fiveTuple_index(tuple);
				boardUnits[pos].tuple_index.push_back(index);
				if(0 == fiveTuples.count(index)){
					fiveTuples.emplace(index, tuple);
				}
			}
			if(xpos - i >= 0 && ypos + i < BOARD_SIZE && xpos - i + 4 < BOARD_SIZE && ypos + i - 4 >= 0){
				for(int j = 0; j < 5; ++j){
					tuple[j] = POSITION(xpos - i + j, ypos + i - j);
				}
				int index = fiveTuple_index(tuple);
				boardUnits[pos].tuple_index.push_back(index);
				if(0 == fiveTuples.count(index)){
					fiveTuples.emplace(index, tuple);
				}
			}
		}
	}
}

int Gobang::fiveTuple_index(const FiveTuple& fiveTuple){
	return (fiveTuple[0] << 8) + fiveTuple[4];
}

bool Gobang::check_win(ChessType type, int xpos, int ypos){
	assert(type == BC || type == WC);
	assert(xpos >= 0 && xpos < BOARD_SIZE && ypos >= 0 && ypos < BOARD_SIZE);
	auto judge = [type, this](int p) {
		return type == BC && board[XPOS(p)][YPOS(p)] > 0 
		|| type == WC && board[XPOS(p)][YPOS(p)] < 0;
	}; 
	int pos = POSITION(xpos, ypos);
	for(int index : boardUnits[pos].tuple_index){
		if(5 == std::count_if(fiveTuples[index].begin(), fiveTuples[index].end(), judge)){
			return true;
		}
	}
	return false;
}

std::vector<int> Gobang::neighbor(int xpos, int ypos, int step){
	assert(IN_BOARD(xpos, ypos));
	assert(step > 0);
	std::vector<int> res;
	int left = std::max(xpos - step, 0), 
		right = std::min(xpos + step, BOARD_SIZE - 1),
		up = std::max(ypos - step, 0),
		down = std::min(ypos + step, BOARD_SIZE - 1);
	for(int x = left; x <= right; ++x){
		for(int y = up; y<= down; ++y){
			res.emplace_back(POSITION(x, y));
		}
	}
	return res;
}

std::set<int> Gobang::extend_openlist(int xpos, int ypos){
	assert(IN_BOARD(xpos, ypos));
	assert(0 != board[xpos][ypos]);
	std::set<int> oldlist = openlist;
	std::vector<int> neighbors = neighbor(xpos, ypos, 1);
	for(auto pos : neighbors){
		if(0 == board[XPOS(pos)][YPOS(pos)]){
			openlist.insert(pos);
		}
	}
	openlist.erase(POSITION(xpos, ypos));
	return oldlist;
}



int Gobang::evaluation(ChessType type){
	assert(type == WC || type == BC);
	int value = 0;
	for(auto & [_, tuple] : fiveTuples){
		int n = 0;
		// count the same type chess
		for(int i = 0; i < 5; ++i){
			if(IS_SAME_TYPE(board[XPOS(tuple[i])][YPOS(tuple[i])], type)){
				++n;
			}else if(IS_SAME_TYPE(board[XPOS(tuple[i])][YPOS(tuple[i])], OPP(type))){
				n = 0;
				break;
			}
		}
		// std::cout<<n;
		value += shape_score[n];
		// std::cout<<value<<std::endl;
		// if(n > 0){
		// 	for(int i = 0; i < 5 ; ++i){
		// 		std::cout<<"("<<XPOS(tuple[i])<<","<<YPOS(tuple[i])<<")";
		// 	}
		// 	std::cout<<"type"<<type<<" "<<n<<std::endl;
		// }
	}
	// system("pause");
	return value;
}

int Gobang::greedy_select(){
	int res = 0;
	int max = INT_MIN;
	// std::cout<<"openlist: ";
	for(int pos : openlist){
		assert(0 == board[XPOS(pos)][YPOS(pos)]);
		board[XPOS(pos)][YPOS(pos)] = order * computer;
		int h1 = evaluation(computer), h2 =  evaluation(player);
		int h = h1 - h2;
		// std::cout<<"("<<XPOS(pos)<<","<<YPOS(pos)<<","<<h1<<","<<h2<<") ";
		if(h > max){
			max = h;
			res = pos;
		}
		board[XPOS(pos)][YPOS(pos)] = 0;
	}
	// std::cout<<std::endl;
	return res;
}

bool Gobang::fall(ChessType type, int xpos, int ypos){
	assert(type == BC || type == WC);
	assert(xpos >= 0 && xpos < BOARD_SIZE && ypos >= 0 && ypos < BOARD_SIZE);
	if(0 != board[xpos][ypos]){
		return false;
	}
	fill_board(type, xpos, ypos);
	extend_openlist(xpos, ypos);
	// DEBUG_OPENLIST(openlist);
	return true;
}

void Gobang::fill_board(ChessType type, int xpos, int ypos){
	assert(type == WC || type == BC);
	assert(IN_BOARD(xpos, ypos));
	assert(0 == board[xpos][ypos]);
	board[xpos][ypos] = order * type;
	// std::cout<<board[xpos][ypos]<<std::endl;
	if(type == WC){
		++order;
	}
}

void Gobang::unfill_board_back(int xpos, int ypos){
	assert(IN_BOARD(xpos, ypos));
	assert(0 != board[xpos][ypos]);
	if(board[xpos][ypos] < 0){
		--order;
	}
	board[xpos][ypos] = 0;
}

void Gobang::minmax(int depth, MinmaxNode & node, int alpha, int beta){
	if(depth == 0 || check_win(node.type, XPOS(node.pos), YPOS(node.pos))){
		int v1 = evaluation(computer), v2 = evaluation(player);
		node.value = v1 - v2;
		return;
	}
	std::vector<int> list(openlist.begin(), openlist.end());
	if(node.type == computer){  // MIN
		node.value = INT_MAX;
		for(int pos : list){
			fill_board(OPP(node.type), XPOS(pos), YPOS(pos));
			std::set<int> oldlist = extend_openlist(XPOS(pos), YPOS(pos));
			MinmaxNode snode(OPP(node.type), pos);
			// the next node is MAX
			// for MAX, beta equals parent's beta
			minmax(depth - 1, snode, INT_MIN, beta);
			// traceback
			unfill_board_back(XPOS(pos), YPOS(pos));
			openlist = oldlist;
			if((node.value > snode.value)){
				node.value = snode.value;
				node.next_best = snode.pos;
			}
			beta = std::min(beta, node.value);
			if(beta < alpha){
				break;
			}
		}
	} else if(node.type == player){  // MAX
		node.value = INT_MIN;
		for(int pos : list){
			fill_board(OPP(node.type), XPOS(pos), YPOS(pos));
			std::set<int> oldlist = extend_openlist(XPOS(pos), YPOS(pos));
			MinmaxNode snode(OPP(node.type), pos);
			minmax(depth - 1, snode, alpha, INT_MAX);
			// traceback
			unfill_board_back(XPOS(pos), YPOS(pos));
			openlist = oldlist;
			if((node.value < snode.value)){
				node.value = snode.value;
				node.next_best = snode.pos;
			}
			alpha = std::max(alpha, node.value);
			if(alpha > beta){
				break;
			}
		}
	}
}

int Gobang::minmax_select(int pre_pos){
	MinmaxNode node(player, pre_pos);
	auto start_time = std::chrono::steady_clock::now();
	minmax(3, node, INT_MIN, INT_MAX);
	auto end_time = std::chrono::steady_clock::now();
  	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
 	std::cout <<"Minmax search 3 depth, "<<ms<<" ms consume"<<std::endl;
	return node.next_best;
}

void Gobang::print(){
	std::cout<<std::setw(3)<<" ";
	for(int i = 0; i <BOARD_SIZE; ++i){
		std::cout<<std::setw(3)<<i;
	}
	std::cout<<std::endl;
	for(int i = 0; i < BOARD_SIZE; ++i){
		std::cout<<std::setw(3)<<i;
		for(int j = 0; j < BOARD_SIZE; ++j){
			if(board[i][j] > 0){
				std::cout<<std::setw(3)<<"O";
			}else if(board[i][j] < 0){
				std::cout<<std::setw(3)<<"X";
			}else{
				std::cout<<std::setw(3)<<".";
			}
		}
		std::cout<<std::endl;
	}
}

void Gobang::run(){
	ChessType cur = BC;
	int fall_pos;
	while(true){
		print();
		if(cur == player){
			int x, y;
			std::cin>>x>>y;
			fall_pos = POSITION(x, y);
		}else{
			// fall_pos = greedy_select();
			fall_pos = minmax_select(fall_pos);
		}
		fall(cur, XPOS(fall_pos), YPOS(fall_pos));
		if(check_win(cur, XPOS(fall_pos), YPOS(fall_pos))){
			break;
		}
		cur = OPP(cur);
	}
}