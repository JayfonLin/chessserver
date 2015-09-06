/*
Created on 2015-09-07

@author:jeff
*/

#include <cstdio>
#include <iostream>
#include "chess_move.h"
#include "chess_util.h"
#include "constant.h"

using std::cout;
using std::endl;

static void printboard(int squares[]){
	for (int i = 0; i < BOARD_NUMBER; ++i){
		
		if (i % 16 == 0)
			printf("\n\n");
		if (!CChessUtil::InBoard(i))
			continue;
		printf("%d\t", squares[i]);

	}
	printf("\n\n");
}

static int makemove(int squares[], CHESS_MOVE move){
	int chess_id;
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	chess_id = squares[sq_dst];
	if (squares[sq_src] == 0){
		cout << "source should have chess!" << endl;
	}
	squares[sq_dst] = squares[sq_src];
	squares[sq_src] = 0;
	move.m_chess_id = chess_id;
	return chess_id;
}