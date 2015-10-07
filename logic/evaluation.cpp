/*
Created on 2015-09-04

@author:jeff
*/

#include "evaluation.h"
#include "constant.h"
#include "chess_util.h"
#include "move_generator.h"

#include <cstring>

int CEvaluator::ProtectAttack(int squares[], bool is_red_turn, int depth){
	int chess_type, target_type;
	int side = is_red_turn ? 0 : 1;
	int i, k;

	for (i = 0; i < BOARD_NUMBER; ++i){
		if (!CChessUtil::InBoard(i)){
			continue;
		}

		if (squares[i] != 0){
			chess_type = squares[i];
			GetRelatePiece(squares, i, side);

			for (k = 0; k < m_pos_count; ++k){
				target_type = squares[m_relate_pos[k]];
				if (target_type == 0){
					m_flexibility_pos[i]++;
				}else{
					if (CChessUtil::SameSide(chess_type, target_type)){
						m_guard_pos[m_relate_pos[k]]++;
					}else{
						m_attack_pos[m_relate_pos[k]]++;
						m_flexibility_pos[i]++;
						switch (target_type){
						case R_KING:
							if (!is_red_turn)
								return LOSS_SCORE + depth;

						case B_KING:
							if (is_red_turn)
								return LOSS_SCORE + depth;

						default:
							m_attack_pos[m_relate_pos[k]] += (30 + (m_base_value[target_type]
								- m_base_value[chess_type])/10)/10;
							break;
						}
					}
				}
			}
		}
	}

	return 0;
}

void CEvaluator::GetFlexibility(int squares[]){
	int i, chess_type;
	for (i = 0; i < BOARD_NUMBER; ++i){
		if (!CChessUtil::InBoard(i)){
			continue;
		}

		if (squares[i] != 0){
			chess_type = squares[i];
			m_chess_value[i]++;
			m_chess_value[i] += m_flex_value[chess_type] * m_flexibility_pos[i];

			//TODO
			//m_chess_value[i] += GetPosValue(i, chess_type)
		}
	}
}

int CEvaluator::GetChessValues(int squares[], bool is_red_turn, int depth){
	int half_value, i, chess_type;
	for (i = 0; i < BOARD_NUMBER; ++i){
		if (!CChessUtil::InBoard(i)){
			continue;
		}

		if (squares[i] == 0){
			continue;
		}

		chess_type = squares[i];
		half_value = m_base_value[chess_type]/16;

		//this code maybe removed
		m_chess_value[i] += m_base_value[chess_type];

		if (CChessUtil::IsRed(chess_type)){
			if (m_attack_pos[i] != 0){
				if (is_red_turn){
					if (chess_type == R_KING){
						m_chess_value[i] -= 20;
					}else{
						m_chess_value[i] -= half_value * 2;
						if (m_guard_pos[i] != 0)
							m_chess_value[i] += half_value;
					}
				}else{
					if (chess_type == R_KING)
						return LOSS_SCORE + depth;

					m_chess_value[i] -= half_value * 10;
					if (m_guard_pos[i] != 0)
						m_chess_value[i] += half_value*9;
				}
				m_chess_value[i] -= m_attack_pos[i];
			}else{
				if (m_guard_pos[i] != 0)
					m_chess_value[i] += 5;
			}
		}else{
			if (m_attack_pos[i] != 0){
				if (!is_red_turn){
					if (chess_type == B_KING){
						m_chess_value[i] -= 20;
					}
					else{
						m_chess_value[i] -= half_value * 2;
						if (m_guard_pos[i] != 0)
							m_chess_value[i] += half_value;
					}
				}
				else{
					if (chess_type == B_KING)
						return LOSS_SCORE + depth;
					m_chess_value[i] -= half_value*10;
					if (m_guard_pos[i] != 0)
						m_chess_value[i] += half_value*9;
				}
				m_chess_value[i] -= m_attack_pos[i];
			}
			else{
				if (m_guard_pos[i] != 0)
					m_chess_value[i] += 5;
			}
		}
	}

	return 0;
}

inline void CEvaluator::AddPoint(int mv){
	int sq_dst = CChessUtil::Dst(mv);
	m_relate_pos[m_pos_count++] = sq_dst;
}

int  CEvaluator::GetScore(int squares[], bool is_red_turn){
	int red_value = 0, black_value = 0, chess_type, i;

	for(i = 0; i < BOARD_NUMBER; i++){
		if (!CChessUtil::InBoard(i)){
			continue;
		}
		
		chess_type = squares[i];
		if (chess_type == 0){
			continue;
		}
			
		if (CChessUtil::IsRed(chess_type))
		{
			red_value += m_chess_value[i];
		}
		else
		{
			black_value += m_chess_value[i];	
		}
	}
		
	if (is_red_turn){
		return red_value - black_value;
	}
	return  black_value - red_value ;
}

int  CEvaluator::GetRelatePiece(int squares[], int sq_src, int side){
	m_pos_count = 0;
	int chess_id;
	bool illeagal = true;
	int* mvs;
	
	chess_id = squares[sq_src] & 0x7;
	int mv;
	switch(chess_id)
	{
	case PIECE_KING:
		mvs = CMoveGenerator::GenKingMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		
		mv = CMoveGenerator::GenKingFace(squares, sq_src);
		if (mv != -1){
			AddPoint(mv);
		}
		
		break;
				
	case PIECE_BODYGUARD:
		mvs = CMoveGenerator::GenBodyguardMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
				
	case PIECE_BISHOP:
		mvs = CMoveGenerator::GenBishopMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
		
	
	case PIECE_HORSE:		
		mvs = CMoveGenerator::GenHorseMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
		
	case PIECE_ROOK:
		mvs = CMoveGenerator::GenRookMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
	
	case PIECE_PAWN:
		mvs = CMoveGenerator::GenPawnMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
		
	case PIECE_CANNON:
		mvs = CMoveGenerator::GenCannonMoves(squares, sq_src, side, illeagal);
		AddPoints(mvs);
		break;
		
	default:
		break;
		
	}
	
	return m_pos_count;
}

void CEvaluator::AddPoints(int mvs[]){
	int sq_dst;
	int mv;
	for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; i++){
		mv = mvs[i];
		if (mv == -1){
			break;
		}
		sq_dst = CChessUtil::Dst(mv);
		m_relate_pos[m_pos_count++] = sq_dst;
	}
}

int CEvaluator::GetPosValue(int sq, int pc){
	if (CChessUtil::IsRed(pc)){
		return PIECE_POS_VALUE[pc&0x7][sq];
	}else{
		return PIECE_POS_VALUE[pc&0x7][CChessUtil::SquareFlip(sq)];
	}
}

CEvaluator::CEvaluator(){
	m_base_value[B_KING] = BASEVALUE_KING; 
	m_base_value[B_ROOK] = BASEVALUE_ROOK; 
	m_base_value[B_HORSE] = BASEVALUE_HORSE; 
	m_base_value[B_BODYGUARD] = BASEVALUE_BODYGUARD; 
	m_base_value[B_BISHOP] = BASEVALUE_BISHOP; 
	m_base_value[B_CANNON] = BASEVALUE_CANNON; 
	m_base_value[B_PAWN] = BASEVALUE_PAWN; 

	m_base_value[R_KING] = BASEVALUE_KING; 
	m_base_value[R_ROOK] = BASEVALUE_ROOK; 
	m_base_value[R_HORSE] = BASEVALUE_HORSE; 
	m_base_value[R_BODYGUARD] = BASEVALUE_BODYGUARD; 
	m_base_value[R_BISHOP] = BASEVALUE_BISHOP; 
	m_base_value[R_CANNON] = BASEVALUE_CANNON; 
	m_base_value[R_PAWN] = BASEVALUE_PAWN; 

	m_flex_value[B_KING] = FLEXIBILITY_KING; 
	m_flex_value[B_ROOK] = FLEXIBILITY_ROOK; 
	m_flex_value[B_HORSE] = FLEXIBILITY_HORSE; 
	m_flex_value[B_BODYGUARD] = FLEXIBILITY_BODYGUARD; 
	m_flex_value[B_BISHOP] = FLEXIBILITY_BISHOP; 
	m_flex_value[B_CANNON] = FLEXIBILITY_CANNON; 
	m_flex_value[B_PAWN] = FLEXIBILITY_PAWN; 

	m_flex_value[R_KING] = FLEXIBILITY_KING; 
	m_flex_value[R_ROOK] = FLEXIBILITY_ROOK; 
	m_flex_value[R_HORSE] = FLEXIBILITY_HORSE; 
	m_flex_value[R_BODYGUARD] = FLEXIBILITY_BODYGUARD; 
	m_flex_value[R_BISHOP] = FLEXIBILITY_BISHOP; 
	m_flex_value[R_CANNON] = FLEXIBILITY_CANNON; 
	m_flex_value[R_PAWN] = FLEXIBILITY_PAWN;
}

CEvaluator::~CEvaluator(){
	
}

int CEvaluator::Evaluate(int squares[], bool is_red_turn, int depth){
	memset(m_chess_value, 0, sizeof(m_chess_value));
	memset(m_attack_pos, 0, sizeof(m_attack_pos));
	memset(m_guard_pos, 0, sizeof(m_guard_pos));
	memset(m_flexibility_pos, 0, sizeof(m_flexibility_pos));

	int score = ProtectAttack(squares, is_red_turn, depth);
	if (score != 0){
		return score;
	}

	GetFlexibility(squares);
	score = GetChessValues(squares, is_red_turn, depth);
	if (score != 0){
		return score;
	}

	return GetScore(squares, is_red_turn);
}