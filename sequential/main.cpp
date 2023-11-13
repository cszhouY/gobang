/*
* @Author: Zhou Yee
* @Date:   2023-04-09 21:16:54
* @Last Modified by:   zy
* @Last Modified time: 2023-11-13 17:22:52
*/
#include "gobang.h"
#include "gobangWidget.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <assert.h>

#define GRAPHICAL

int main(int argc, char const *argv[])
{
	// srand(time(nullptr));
#ifdef GRAPHICAL
	GobangWidget widget;
	ChessType player = BC;
	widget.menu(player);
	Gobang gobang(player);
	widget.draw_board();

	ChessType cur = BC;
	int fall_pos = 0;
	while(true){
		widget.draw_turn(cur);
		if(player == cur){
			while(true){
				fall_pos = widget.get_mouse_click_pos();
				if(gobang.fall(cur, XPOS(fall_pos), YPOS(fall_pos))){
					break;
				}
			}
		}else{
			// fall_pos = greedy_select();
			// fall_pos = gobang.minmax_select(fall_pos);
			fall_pos = gobang.iteration_deeping_search(fall_pos);
			bool flag = gobang.fall(cur, XPOS(fall_pos), YPOS(fall_pos));
			assert(flag);
		}
		widget.draw_chess(cur, XPOS(fall_pos), YPOS(fall_pos));
		if(gobang.check_win(cur, XPOS(fall_pos), YPOS(fall_pos))){
			widget.win_message(cur);
			break;
		}
		cur = OPP(cur);
	}
#else
	Gobang gobang(BC);
	gobang.run();
#endif
	return 0;
}