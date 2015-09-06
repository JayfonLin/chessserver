/*
Created on 2015-09-06

@author:jeff
*/

#include "negascout_tt_hh.h"
#include "board.cpp"
#include "test_util.cpp"

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
	CNegaScout_TT_HH* engine = new CNegaScout_TT_HH();
	CTestBoard boards;

	engine->SearchAGoodMove(boards.boards[0]);
	printboard(boards.boards[0]);
	CHESS_MOVE best_move = engine->GetBestMove();
	makemove(boards.boards[0], best_move);
	printboard(boards.boards[0]);

	system("pause");
	return 0;
}