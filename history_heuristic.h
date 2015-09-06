#ifndef HISTORY_HEURISTIC_H
#define HISTORY_HEURISTIC_H

#include "chess_move.h"



class CHistoryHeuristic
{
public:
	const static int HISTORY_MOVE_SIZE = 1 << 16;

protected:
	
	int m_history_table[HISTORY_MOVE_SIZE];
	CHESS_MOVE m_target_buff[100];

	void Merge(CHESS_MOVE source[], CHESS_MOVE target[], int l, int m, int r);
	void MergePass(CHESS_MOVE source[], CHESS_MOVE target[], int s, int n, bool direction);
	void Merge_A(CHESS_MOVE source[], CHESS_MOVE target[], int l, int m, int r);

public:
	
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();
	void ResetHistoryTable();
	int GetHistoryScore(CHESS_MOVE move);
	void EnterHistoryScore(CHESS_MOVE move, int depth);
	void MergeSort(CHESS_MOVE source[], int n, bool direction);

};

#endif