#include "gobangWidget.h"

#include <cmath>
#include <iostream>

void GobangWidget::menu(ChessType & player){
	initgraph(300, 400);
	setbkcolor(RGB(255, 165, 0));
	cleardevice();
	setfillcolor(RGB(0,0,0));
	fillcircle(300 / 2, 400 / 4, 50);
	setfillcolor(RGB(255,255,255));
	fillcircle(300 / 2, 400 / 4 * 3, 50);
	// choose the chess
	while(true){
		if(MouseHit()){
			MOUSEMSG m = GetMouseMsg();
	        if (m.uMsg == WM_LBUTTONDOWN) { 
	            if(std::hypot(m.x - 300 / 2, m.y - 400 / 4) <= 50){
	            	player = BC;
	            	break;
	            }else if(std::hypot(m.x - 300 / 2, m.y - 400 / 4 * 3) <= 50){
	            	player = WC;
	            	break;
	            }
			}else if(m.uMsg == WM_CLOSE || m.uMsg == WM_QUIT || m.uMsg == WM_DESTROY){
				SendMessage(GetHWnd(), WM_QUIT, 0, 0);
	            exit(0);
			}
		}
	}
	closegraph();
}

void GobangWidget::draw_board(){
	initgraph(width, height);	
	setbkcolor(RGB(255, 255, 204));
	cleardevice();
	// draw lines
	setlinestyle(PS_SOLID, 2);
	setcolor(RGB(0,0,0));
	for(int i=1; i <= BOARD_SIZE; ++i){
		line(i*step, 1*step, i*step,  BOARD_SIZE*step);
		line(1*step, i*step, BOARD_SIZE*step, i*step);
	}
	// text
	settextcolor(BLACK);
    outtextxy(650, 100, "Turn");
}

void GobangWidget::draw_chess(ChessType chessType, int xpos, int ypos){
	if(chessType == BC){
		setfillcolor(RGB(0,0,0));
	}else{
		setfillcolor(RGB(255, 255, 255));
	}
	fillcircle((ypos + 1) * step, (xpos + 1) * step, step / 2 + 0.8);
}

int GobangWidget::get_mouse_click_pos(){
	int xpos = -1, ypos = -1;
	do{
		if(MouseHit()){
			MOUSEMSG m = GetMouseMsg();
	        if (m.uMsg == WM_LBUTTONDOWN) { 
	            ypos = round((float)m.x / step) - 1;
	            xpos = round((float)m.y / step) - 1;
			}else if(m.uMsg == WM_CLOSE || m.uMsg == WM_QUIT || m.uMsg == WM_DESTROY){
				SendMessage(GetHWnd(), WM_QUIT, 0, 0);
	            exit(0);
			}
		}
	}while(!IN_BOARD(xpos, ypos));
	return POSITION(xpos, ypos);
}

void GobangWidget::win_message(ChessType chessType){
	if(chessType == BC){
		MessageBox(GetHWnd(), "Black win!", "Game Over", MB_OK);
	}else{
		MessageBox(GetHWnd(), "White win!", "Game Over", MB_OK);
	}
}

void GobangWidget::draw_turn(ChessType cur){
	clearcircle(700, 200, 50);
	if(cur == BC){
		setfillcolor(RGB(0,0,0));
	}else{
		setfillcolor(RGB(255, 255, 255));
	}
	fillcircle(700, 200, 40);
}