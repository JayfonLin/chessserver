/*
Created on 2015-09-06

@author:jeff
*/

#include "negascout_tt_hh.h"
#include "board.cpp"
#include "test_util.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
	CNegaScout_TT_HH* engine = new CNegaScout_TT_HH();
	CTestBoard boards;

	for (int i = 0; i < 4; ++i){
		engine->SearchAGoodMove(boards.boards[i]);
		CTestUtil::PrintBoard(boards.boards[i]);
		CHESS_MOVE best_move = engine->GetBestMove();
		CTestUtil::MakeMove(boards.boards[i], best_move);
		system("pause");
		system("cls");
		CTestUtil::PrintBoard(boards.boards[i]);
		system("pause");
		system("cls");
	}

	return 0;
}