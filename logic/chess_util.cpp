
/*
Created on 2015-10-11

@author: jeff
*/

#include "chess_util.h"

int CChessUtil::IsGameOver(int squares[]){
	bool red_live = false, black_live = false;
	for (int i = 0; i < BOARD_NUMBER; ++i){
		if (!CChessUtil::InFort(i)){
			continue;
		}
		if (squares[i] == R_KING){
			red_live = true;
		}
		if (squares[i] == B_KING){
			black_live = true;
		}
	}
	if (red_live && black_live)
		return 0;
	if (red_live)
		return 1;
	return -1;
}
