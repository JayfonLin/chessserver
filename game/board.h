
#ifndef BOARD_H
#define BOARD_H

/*
Created on 2015-10-05

@author: jeff
*/

#include "../logic/constant.h"
#include "../logic/chess_move.h"

class CBoard
{
private: 
	int m_cur_board[BOARD_NUMBER];

public:
	CBoard();
	virtual ~CBoard();
	void LoadBoard(const int* board);
	void LoadStartupBoard();
	int MakeMove(struct CHESS_MOVE move);
	void UnmakeMove();
	int* GetCurBoard();

};

CBoard* GetBoardInstance();



#endif