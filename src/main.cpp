/*
* @Author: Zhou Yee
* @Date:   2023-04-09 21:16:54
* @Last Modified by:   zy
* @Last Modified time: 2023-04-17 08:52:51
*/
#include "gobang.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char const *argv[])
{
	srand(time(nullptr));
	Gobang gobang(BLACK);
	gobang.run();
	return 0;
}