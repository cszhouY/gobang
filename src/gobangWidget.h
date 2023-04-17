#ifndef _GOBANG_WIDGET_H
#define _GOBANG_WIDGET_H

#include "gobang.h"

#include <graphics.h>
#include <conio.h>

class GobangWidget{
public:
	GobangWidget(): width(800), height(600){
		step = 600 / (BOARD_SIZE + 1);
	}
public:
	void menu(ChessType & player);
	void draw_board();
	void draw_chess(ChessType chessType, int xpos, int ypos);
	int get_mouse_click_pos();
	void win_message(ChessType chessType);
	void draw_turn(ChessType cur);
private:
	int width, height;
	int step;
};

#endif