/*
Created on 2015-09-06
@author:jeff
*/

#include "constant.h"
#include <cstring>


class CTestBoard
{
public:
	int boards[1][BOARD_NUMBER];

	CTestBoard(){
		memcpy(boards[0], BOARD_STARTUP, sizeof(boards[0]));
	}
};

