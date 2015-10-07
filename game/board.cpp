
/*
Created on 2015-10-05

@author: jeff
*/

#include "board.h"
#include <cstring>
#include "../logic/chess_util.h"

#include <iostream>

CBoard::CBoard(){
	LoadStartupBoard();
}

CBoard::~CBoard(){}

void CBoard::LoadBoard(const int* board){
	memcpy(m_cur_board, board, sizeof(m_cur_board));
}

void CBoard::LoadStartupBoard(){
	LoadBoard(BOARD_STARTUP);
}

int CBoard::MakeMove(struct CHESS_MOVE move){
	int chess_id;
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	chess_id = m_cur_board[sq_dst];
	if (m_cur_board[sq_src] == 0){
		std::cout << "source should have chess!" << std::endl;
	}
	m_cur_board[sq_dst] = m_cur_board[sq_src];
	m_cur_board[sq_src] = 0;
	move.m_chess_id = chess_id;
	return chess_id;
}

void CBoard::UnmakeMove(){}

int* CBoard::GetCurBoard(){
	return m_cur_board;
}

static CBoard* instance = new CBoard();
CBoard* GetBoardInstance(){
	return instance;
}