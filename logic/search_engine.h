#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H
/*
Created on 2015-09-03

@author:jeff
*/

#include "constant.h"
#include "chess_move.h"
#include "move_generator.h"
#include "evaluation.h"
#include "chess_util.h"

#include <iostream>
using std::cout;
using std::endl;

class CSearchEngine
{
protected:
	int m_cur_position[BOARD_NUMBER];
	CHESS_MOVE m_best_move;
	CMoveGenerator* m_move_generator;
	CEvaluator* m_evaluator;
	int m_search_depth;
	int m_max_depth;
	int m_move_count;
	CHESS_MOVE m_move_list[10][80];

	void InitMoveList();
	virtual CHESS_MOVE SearchAGoodMove(int squares[]) = 0;
	virtual int MakeMove(CHESS_MOVE move);

	virtual void UnmakeMove(CHESS_MOVE move, int chess_id);
	virtual int IsGameOver(int squares[], int depth);

	const static int DEFAULT_SEARCH_DEPTH = 4;

public:
	CSearchEngine();
	virtual ~CSearchEngine();
	void SetBestMove(CHESS_MOVE move);
	CHESS_MOVE GetBestMove();
	void SetSearchDepth(int depth);
	void SetEvaluator(CEvaluator* evaluator);
	void SetMoveGenerator(CMoveGenerator* generator);

};

#endif