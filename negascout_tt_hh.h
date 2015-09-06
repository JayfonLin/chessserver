#ifndef NEGASCOUT_TT_HH_H
#define NEGASCOUT_TT_HH_H
/*
Created on 2015-09-05

@author:jeff
*/

#include "search_engine.h"
#include "transposition_table.h"
#include "history_heuristic.h"

class CNegaScout_TT_HH: public CSearchEngine
{
protected:
	CTranspositionTable* m_tt;
	CHistoryHeuristic* m_hh;
	int m_moves[80];

	int NegaScout(int depth, int alpha, int beta);
	void AddMoves(int count, int ply);

public:
	bool SearchAGoodMove(int squares[]);
	CNegaScout_TT_HH();
	virtual ~CNegaScout_TT_HH();

	friend class CHistoryHeuristic;
};


#endif