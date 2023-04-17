#ifndef GOBANG
#define GOBANG

#define BOARD_SIZE 15

#define POSITION(x, y) ((x) * BOARD_SIZE + (y)) 
#define XPOS(pos) ((pos) / BOARD_SIZE)
#define YPOS(pos) ((pos) % BOARD_SIZE)
#define IS_SAME_TYPE(chess, type) (chess != 0 && ((type == BC && chess > 0) || (type == WC && chess < 0)))
#define OPP(type) (ChessType(-type))
#define IN_BOARD(xpos, ypos) (xpos >= 0 && xpos < BOARD_SIZE && ypos >= 0 && ypos < BOARD_SIZE)

#define DEBUG_OPENLIST(set) \
	std::cout<<"openlist: ";\
	for(int pos : set){\
		std::cout<<"("<<XPOS(pos)<<","<<YPOS(pos)<<")";\
	}\
	std::cout<<std::endl;

#include <vector>
#include <array>
#include <set>
#include <unordered_map>

typedef std::array<int, 5> FiveTuple;

template <class T>
void set_intersection(std::set<T> & src, const std::set<T> & set){
	std::set<T>::iterator it = set.begin();
	for(;it != set.end(); ++it){
		src.erase(*it);
	}
}

// extern class BoardUnit;

enum ChessType{
	ZERO = 0, BC = 1, WC = -1
};

struct BoardUnit{
	int xpos, ypos;
	std::vector<int> tuple_index;
};

struct MinmaxNode{
	ChessType type; // 
	int pos;
	int value;
	int next_best;
	MinmaxNode(ChessType type, int pos): type(type), pos(pos){}
	bool operator < (const MinmaxNode & node) const {
		return value < node.value;
	}
};

class Gobang{
public:
	Gobang(ChessType player);
public:
	bool check_win(ChessType type, int xpos, int ypos);
	bool fall(ChessType type, int xpos, int ypos);
	void fill_board(ChessType type, int xpos, int ypos);
	void unfill_board_back(int xpos, int ypos);
	std::vector<int> neighbor(int xpos, int ypos, int step);
	std::set<int> extend_openlist(int xpos, int ypos);
	int evaluation(ChessType type);
	int greedy_select();
	void minmax(int depth, MinmaxNode & node, int alpha, int beta);
	int minmax_select(int pre_pos);
	void print();
	void run();
private:
	void init_fiveTuples();
	int fiveTuple_index(const FiveTuple& fiveTuple);
private:
	std::vector<BoardUnit> boardUnits;
	std::unordered_map<int, FiveTuple> fiveTuples;
	std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board;
	std::array<int, 6> shape_score{0, 1, 20, 600, 4000, 1000000};
	std::set<int> openlist;
	ChessType player, computer;
	int order;
};

#endif