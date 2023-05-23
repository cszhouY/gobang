#ifndef GOBANG
#define GOBANG

#define BOARD_SIZE 15

#define MINMAX_DEPTH 6



#define POSITION(x, y) ((x) * BOARD_SIZE + (y)) 
#define XPOS(pos) ((pos) / BOARD_SIZE)
#define YPOS(pos) ((pos) % BOARD_SIZE)
#define IS_SAME_TYPE(chess, type) (chess != 0 && ((type == BC && chess > 0) || (type == WC && chess < 0)))
#define OPP(type) (ChessType(-type))
#define IN_BOARD(xpos, ypos) ((xpos) >= 0 && (xpos) < BOARD_SIZE && (ypos) >= 0 && (ypos) < BOARD_SIZE)

#define RAND_64() (__int64)(((__int64)rand() << 48) | ((__int64)rand() << 32) | ((__int64)rand() << 16) | ((__int64)rand()))

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

enum ShapeType{
	T_NO = 0, T_ONE, T_TWO, T_STHR, T_LTHR, T_CFOUR, T_LFOUR, T_FIVE
};

struct Zobrist{
	__int64 zBlack, zWhite, zZero;
};

struct MinmaxNode{
	ChessType type; // 
	int depth;
	int pos;
	int value;
	int evalue;
	int next_best;
	ChessType win;
	MinmaxNode(ChessType type, int pos): type(type), pos(pos), win(ZERO){}
	MinmaxNode(ChessType type, int pos, int evalue): type(type), pos(pos), evalue(evalue), win(ZERO){}
	MinmaxNode(ChessType type, int pos, int evalue, int depth)
	: type(type), pos(pos), evalue(evalue), depth(depth), win(ZERO){}
	bool operator < (const MinmaxNode & node) const {
		return evalue < node.evalue;
	}
	bool operator > (const MinmaxNode & node) const {
		return evalue > node.evalue;
	}
};

typedef void(* MinmaxFunct)(int depth, MinmaxNode & node, int alpha, int beta);

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
	int total_evaluate(ChessType type);
	int pos_evaluate(ChessType type, int xpos, int ypos);
	int tuple_evaluate(ChessType type, const FiveTuple & fiveTuple);
	int pos_evaluate_inc(ChessType type, int xpos, int ypos);
	int greedy_select();
	void minmax_v1(int depth, MinmaxNode & node, int alpha, int beta);
	void minmax_v2(int depth, MinmaxNode & node, int alpha, int beta);

	// int minimax()

	void minmax_kill(int depth, MinmaxNode & node, int alpha, int beta);
	int minmax_select(int pre_pos);
	int iteration_deeping_search(int pre_pos);
	void print();
	void run();
private:
	void init_fiveTuples();
	int fiveTuple_index(const FiveTuple& fiveTuple);
	void init_zobristBoard();
	ShapeType tuple_shape_type(ChessType type, const FiveTuple & fiveTuple);
	bool is_kill_pos(ChessType chessType, int xpos, int ypos);
	bool is_defend_pos(ChessType chessType, int xpos, int ypos);
private:
	std::vector<BoardUnit> boardUnits;
	std::unordered_map<int, FiveTuple> fiveTuples;
	std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board;
	std::unordered_map<__int64, int> evalMap;
	std::array<std::array<Zobrist, BOARD_SIZE>, BOARD_SIZE> zobristBoard;
	std::array<int, 8> shape_score{0, 1, 20, 500, 1200, 4000, 100000, 1000000};
	std::set<int> openlist;
	ChessType player, computer;
	int order;
	__int64 cur_zobrist;
};

#endif