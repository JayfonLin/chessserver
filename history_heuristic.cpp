/*
Created on 2015-09-05

@author:jeff
*/

#include "history_heuristic.h"

void CHistoryHeuristic::Merge(CHESS_MOVE source[], CHESS_MOVE target[], int l, int m, int r){
	int i = l;
	int j = m + 1;
	int k = l;
	
	while((i <= m) && (j <= r))
		if (source[i].m_score <= source[j].m_score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];
		
		if(i > m)
			for (int q = j; q <= r; q++)
				target[k++] = source[q];
			else
				for(int q = i; q <= m; q++)
					target[k++] = source[q];
}

void CHistoryHeuristic::MergePass(CHESS_MOVE source[], CHESS_MOVE target[], int s, int n, bool direction){
	int i = 0;
	
	while(i <= n - 2 * s)
	{
		if (direction)
			Merge(source, target, i, i + s - 1, i + 2 * s - 1);
		else
			Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);
		i=i+2*s;
	}
	
	if (i + s < n) 
	{
		if (direction)
			Merge(source, target, i, i + s - 1, n - 1);
		else
			Merge_A(source, target, i, i + s - 1, n - 1);
	}
	else
		for (int j = i; j <= n - 1; j++)
			target[j] = source[j];
}
void CHistoryHeuristic::Merge_A(CHESS_MOVE source[], CHESS_MOVE target[], int l, int m, int r){
	int i = l;
	int j = m + 1;
	int k = l;
	
	while((i <= m) && (j <= r))
		if (source[i].m_score >= source[j].m_score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];
		
	if(i > m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for(int q = i; q <= m; q++)
			target[k++] = source[q];
}

CHistoryHeuristic::CHistoryHeuristic(){
}

CHistoryHeuristic::~CHistoryHeuristic(){
}

void CHistoryHeuristic::ResetHistoryTable(){
	for (int i = 0; i < HISTORY_MOVE_SIZE; ++i){
		m_history_table[i] = 10;
	}
}

int CHistoryHeuristic::GetHistoryScore(CHESS_MOVE move){
	return m_history_table[move.m_move];
}

void CHistoryHeuristic::EnterHistoryScore(CHESS_MOVE move, int depth){
	m_history_table[move.m_move] += 2<<depth;
}

void CHistoryHeuristic::MergeSort(CHESS_MOVE source[], int n, bool direction){
	int s = 1;
	while(s < n)
	{
		MergePass(source, m_target_buff, s, n, direction);
		s += s;
		MergePass(m_target_buff, source, s, n, direction);
		s += s;
	}
}