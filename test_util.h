/*
Created on 2015-09-07

@author:jeff
*/

#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <cstdio>
#include <iostream>
#include <string>
#include "chess_move.h"
#include "chess_util.h"
#include "constant.h"

using std::cout;
using std::endl;
using std::string;

static const char* PIECE_CHINESE[PIECE_COUNT] =
{
	"", "", "", "", "", "", "", "",
	"R_KI", "R_BO", "R_BI", "R_HO", "R_RO", "R_CA", "R_PA", "",
	"B_KI", "B_BO", "B_BI", "B_HO", "B_RO", "B_CA", "B_PA", "",

};

class CTestUtil
{

public:
	
	static void PrintBoard(int squares[]){
		for (int i = 0; i < BOARD_NUMBER; ++i){

			if (i % 16 == 0){
				if (CChessUtil::RankY(i) >= RANK_TOP && CChessUtil::RankY(i) <= RANK_BOTTOM){
					printf("\n\n");
				}
			}

			if (!CChessUtil::InBoard(i)){
				continue;
			}

			printf("%5s", PIECE_CHINESE[squares[i]]);

		}
		printf("\n\n");
	}

	static int MakeMove(int squares[], CHESS_MOVE move){
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
};


#endif