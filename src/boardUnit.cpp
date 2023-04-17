/*
* @Author: Zhou Yee
* @Date:   2023-04-04 20:34:54
* @Last Modified by:   zy
* @Last Modified time: 2023-04-16 22:50:38
*/
#include "boardUnit.h"
#include <climits>
#include <assert.h>
#include <iostream>
#include <cmath>


/*BoardUnit::BoardUnit(const int xpos, const int ypos){
	// std::cout<<xpos<<" "<<ypos<<std::endl;
	assert(xpos >= 0 && xpos < BOARD_SIZE && ypos >= 0 && ypos < BOARD_SIZE );
	this->xpos = xpos;
	this->ypos = ypos;
	FiveTuple tuple;
	for(int i = 4; i >= 0; --i){
		if(xpos - i >= 0 && xpos - i + 4 < BOARD_SIZE){
			for(int j = 0; j < 5; ++j){
				tuple[j] = POSITION(xpos - i + j, ypos);
				// std::cout<<"("<<xpos - i + j<<","<<ypos<<")";
				// std::cout<<tuple[j]<<"("<<XPOS(tuple[j])<<","<<YPOS(tuple[j])<<")";
			}
			tuples.emplace_back(tuple);
			// std::cout<<std::endl;
		}
		if(ypos - i >= 0 && ypos - i + 4 < BOARD_SIZE){
			for(int j = 0; j < 5; ++j){
				tuple[j] = POSITION(xpos, ypos - i + j);
				// std::cout<<"("<<xpos<<","<<ypos - i + j<<")";
				// std::cout<<tuple[j]<<"("<<XPOS(tuple[j])<<","<<YPOS(tuple[j])<<")";
			}
			tuples.emplace_back(tuple);
			// std::cout<<std::endl;
		}
		if(xpos - i >= 0 && ypos - i >= 0 && xpos - i + 4 < BOARD_SIZE && ypos - i + 4 < BOARD_SIZE){
			for(int j = 0; j < 5; ++j){
				tuple[j] = POSITION(xpos - i + j, ypos - i + j);
				// std::cout<<"("<<xpos - i + j<<","<<ypos - i + j<<")";
				// std::cout<<tuple[j]<<"("<<XPOS(tuple[j])<<","<<YPOS(tuple[j])<<")";
			}
			tuples.emplace_back(tuple);
			// std::cout<<std::endl;
		}
		if(xpos - i >= 0 && ypos + i < BOARD_SIZE && xpos - i + 4 < BOARD_SIZE && ypos + i - 4 >= 0){
			for(int j = 0; j < 5; ++j){
				tuple[j] = POSITION(xpos - i + j, ypos + i - j);
				// std::cout<<"("<<xpos - i + j<<","<<ypos + i - j<<")";
				// std::cout<<"("<<XPOS(tuple[j])<<","<<YPOS(tuple[j])<<")";
			}
			tuples.emplace_back(tuple);
			// std::cout<<std::endl;
		}
	}
}

std::vector<FiveTuple>& BoardUnit::get_tuples(){
	return tuples;
}*/