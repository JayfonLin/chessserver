/*
Created on 2015-09-04

@author:jeff
*/

#include "search_engine.h"

int CSearchEngine::MakeMove(CHESS_MOVE move){
	int chess_id;
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	chess_id = m_cur_position[sq_dst];
	if (m_cur_position[sq_src] == 0){
		cout << "source should have chess!" << endl;
	}
	m_cur_position[sq_dst] = m_cur_position[sq_src];
	m_cur_position[sq_src] = 0;
	move.m_chess_id = chess_id;
	return chess_id;
}

void CSearchEngine::SetBestMove(CHESS_MOVE move){
	m_best_move = move;
}

CHESS_MOVE CSearchEngine::GetBestMove(){
	return m_best_move;
}

void CSearchEngine::SetSearchDepth(int depth){
	m_search_depth = depth;
}

void CSearchEngine::SetEvaluator(CEvaluator* evaluator){
	m_evaluator = evaluator;
}

void CSearchEngine::SetMoveGenerator(CMoveGenerator* generator){
	m_move_generator = generator;
}

CSearchEngine::CSearchEngine(){
	m_search_depth = DEFAULT_SEARCH_DEPTH;
	m_evaluator = new CEvaluator();
	m_move_generator = new CMoveGenerator();

	InitMoveList();
}

CSearchEngine::~CSearchEngine(){
	delete m_move_generator;
	delete m_evaluator;

	/*
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 80; ++j){
			delete m_move_list[i][j];
		}
	}
	*/
}

void CSearchEngine::InitMoveList(){
	/*
	for (int i = 0; i < 10; ++i){
		for (int j = 0; j < 80; ++j){
			m_move_list[i][j] = new CHESS_MOVE();
		}
	}
	*/
}

void CSearchEngine::UnmakeMove(CHESS_MOVE move, int chess_id){
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	m_cur_position[sq_src] = m_cur_position[sq_dst];
	m_cur_position[sq_dst] = chess_id;
}

int CSearchEngine::IsGameOver(int squares[], int depth){
	int i;
	bool red_live = false, black_live = false; 
	
	for (i = 0; i < BOARD_NUMBER; ++i){
		if (!CChessUtil::InFort(i)){
			continue;
		}
		if (squares[i] == R_KING){
			red_live = true;
		}
		if (squares[i] == B_KING){
			black_live = true;
		}
	}

	i = (m_max_depth - depth + 1) % 2;
	
	if (!red_live)
		if (i != 0)
			return LOSS_SCORE + depth;
		else
			return -LOSS_SCORE - depth;
	if (!black_live)
		if (i != 0)
			return -LOSS_SCORE - depth;
		else
			return LOSS_SCORE + depth;
	return 0;
}

