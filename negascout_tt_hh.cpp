/*
Created on 2015-09-05

@author:jeff
*/

#include "negascout_tt_hh.h"
#include <cstring>

int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta){
	int count,i;
	int type;
	int a,b,t;
	int side;
	int score;
	
	i = IsGameOver(m_cur_position, depth);
	if (i != 0)
		return i;
	
	side = 1-(m_max_depth-depth)%2;
	
	score = m_tt->LookupHashTable(alpha, beta, depth, side);
	if (score != INVALID_SCORE && depth != m_max_depth){
		/*if (depth == m_max_depth){
			ChessMove move = new ChessMove();
			move.Move = m_tt.LookupBestMove(side);
			m_best_move = move;
		}*/
		return score;
	}
	
	if (depth <= 0)	
	{
		score = m_evaluator->Evaluate(m_cur_position, side == 0, depth);
		m_tt->EnterHashTable(EXACT, score, depth, side);
		return score;
	}
	
	//m_nMovecount = 0;
	count = m_move_generator->CreatePossibleMove(m_cur_position, m_moves, depth, side);
	AddMoves(count, depth);
	
	for (i = 0; i < count; ++i) 
	{
		m_move_list[depth][i].m_score = m_hh->GetHistoryScore(m_move_list[depth][i]);
	}
	m_hh->MergeSort(m_move_list[depth], count, false);
	
	int bestmove = -1;
	
    a = alpha;
    b = beta;
    int eval_is_exact = 0;
    for (i = 0; i < count; ++i){
		m_tt->HashMakeMove(m_move_list[depth][i], m_cur_position);
		type = MakeMove(m_move_list[depth][i]);

		t = -NegaScout(depth-1 , -b, -a);

		if (t > a && t < beta && i > 0){
			a = -NegaScout (depth-1, -beta, -t);     /* re-search */
			eval_is_exact = 1; 
			if(depth == m_max_depth){
				m_best_move = m_move_list[depth][i];
				//m_tt.EnterHashBestMove(m_best_move.Move, side);
			}
			bestmove = i; 
		}
		
		m_tt->HashUnmakeMove(m_move_list[depth][i], type, m_cur_position); 
		UnmakeMove(m_move_list[depth][i],type); 
		if (a < t){
			eval_is_exact = 1;
			a = t;
			if(depth == m_max_depth) {
				m_best_move = m_move_list[depth][i];
				//m_tt.EnterHashBestMove(m_best_move.Move, side);
			}
		}
		if (a >= beta) 
		{
			m_tt->EnterHashTable(LOWER_BOUND, a, depth, side);
			m_hh->EnterHistoryScore(m_move_list[depth][i], depth);
			return a;
		}
		b = a + 1;                      /* set new null window */
	}
	if (bestmove != -1)
		m_hh->EnterHistoryScore(m_move_list[depth][bestmove], depth);
	if (eval_is_exact != 0) 
		m_tt->EnterHashTable(EXACT, a, depth, side);
	else 
		m_tt->EnterHashTable(UPPER_BOUND, a, depth, side);
	return a;
}

void CNegaScout_TT_HH::AddMoves(int count, int ply){
	for (int i = 0; i < count; ++i){
		m_move_list[ply][i].m_move = m_moves[i];
	}
}

bool CNegaScout_TT_HH::SearchAGoodMove(int squares[]){
	memcpy(m_cur_position, squares, sizeof(m_cur_position));
	m_max_depth = m_search_depth;
	m_tt->CalculateInitHashKey(m_cur_position);
	m_hh->ResetHistoryTable();

	NegaScout(m_max_depth, -INFINITY_SCORE, INFINITY_SCORE);
	bool is_kill = false;
	int sq_dst = CChessUtil::Dst(m_best_move.m_move);
	if (m_cur_position[sq_dst] != 0)
		is_kill = true;

	int best_score = m_tt->LookupHashTable(-INFINITY_SCORE, INFINITY_SCORE, m_max_depth, 1);
	MakeMove(m_best_move);
	
	memcpy(squares, m_cur_position, sizeof(squares));
	return is_kill;
}

CNegaScout_TT_HH::CNegaScout_TT_HH(){
	m_tt = new CTranspositionTable();
	m_hh = new CHistoryHeuristic();
}

CNegaScout_TT_HH::~CNegaScout_TT_HH(){
	delete m_tt;
	delete m_hh;
}
