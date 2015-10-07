/*
Created on 2015-09-04

@author:jeff
*/

#include "move_generator.h"
#include "chess_util.h"


int CMoveGenerator::m_moves[SINGLE_GEN_MOVE_SIZE];

CMoveGenerator::CMoveGenerator(){
}

CMoveGenerator::~CMoveGenerator(){
}

bool CMoveGenerator::LegalKingFace(int squares[], int mv){
	int sq_src = CChessUtil::Src(mv);
	const int sq_dst = CChessUtil::Dst(mv);
	if (!CChessUtil::SameFile(sq_src, sq_dst)){
		return false;
	}
	int piece_src = squares[sq_src];
	int piece_dst  = squares[sq_dst];
	if (piece_src == piece_dst){
		return false;
	}
	if (piece_src == 0 || piece_dst == 0){
		return false;
	}
	
	int piece_type = piece_src & 0x7;
	if (piece_type != PIECE_KING){
		return false;
	}
	piece_type = piece_dst & 0x7;
	if (piece_type != PIECE_KING){
		return false;
	}
	
	int delta = (sq_dst < sq_src ? -16 : 16);
	sq_src += delta;
	while (CChessUtil::InBoard(sq_src) && sq_src != sq_dst){
		if (squares[sq_src] != 0){
			return false;
		}
		sq_src += delta;
	}
	
	return true;
}

int CMoveGenerator::CreatePossibleMove(int squares[], int mvs[], int ply, int side){

	int sq_src;
	int piece_self_side, piece_src;
	bool illeagal = false; 
	
	m_gen_moves = 0;
	piece_self_side = CChessUtil::SideTag(side);
	for (sq_src = 0; sq_src < BOARD_NUMBER; ++sq_src){
		
		if (!CChessUtil::InBoard(sq_src)){
			continue;
		}

		piece_src = squares[sq_src];
		if ((piece_src & piece_self_side) == 0){
			continue;
		}
		
		int *gen_mvs;
		int mv;
		switch (piece_src - piece_self_side){
		case PIECE_KING:
			gen_mvs = GenKingMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		case PIECE_BODYGUARD:
			gen_mvs = GenBodyguardMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		case PIECE_BISHOP:
			gen_mvs = GenBishopMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		case PIECE_HORSE:
			gen_mvs = GenHorseMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
		case PIECE_ROOK:
			gen_mvs = GenRookMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		case PIECE_CANNON:
			gen_mvs = GenCannonMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		case PIECE_PAWN:
			gen_mvs = GenPawnMoves(squares, sq_src, side, illeagal);
			for (int i = 0; i < SINGLE_GEN_MOVE_SIZE; ++i){
				mv = gen_mvs[i];
				if (mv == -1){
					break;
				}
				mvs[m_gen_moves++] = mv;
			}
			break;
			
		default:
			break;
		}
	}
	return m_gen_moves;
}

int* CMoveGenerator::GenKingMoves(int squares[], int sq_src, int side, bool illeagal){
	int sq_dst, piece_dst, gen_count = 0;
	int piece_self_side = CChessUtil::SideTag(side);

	for (int i = 0; i < 4; ++i){
		sq_dst = sq_src + KING_DELTA[i];
		if (!CChessUtil::InFort(sq_dst)){
			continue;
		}
		piece_dst = squares[sq_dst];
		if ((piece_dst & piece_self_side) == 0 || illeagal){
			m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
		}
	}

	int mv = GenKingFace(squares, sq_src);
	if (mv != -1){
		m_moves[gen_count++] = mv;
	}
	
	m_moves[gen_count] = -1;
	return m_moves;

}

int* CMoveGenerator::GenBodyguardMoves(int squares[], int sq_src, int side, bool illeagal){
	int sq_dst, piece_dst, gen_count = 0;
	int piece_self_side = CChessUtil::SideTag(side);
	
	for (int i = 0; i < 4; ++i){
		sq_dst = sq_src + BODYGUARD_DELTA[i];
		if (!CChessUtil::InFort(sq_dst)){
			continue;
		}
		piece_dst = squares[sq_dst];
		if ((piece_dst & piece_self_side) == 0 || illeagal){
			m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int* CMoveGenerator::GenBishopMoves(int squares[], int sq_src, int side, int illeagal){
	int sq_dst, piece_dst, gen_count = 0;
	int piece_self_side = CChessUtil::SideTag(side);
	
	for (int i = 0; i < 4; ++i){
		sq_dst = sq_src + BODYGUARD_DELTA[i];
		if (!(CChessUtil::InBoard(sq_dst) && CChessUtil::HomeHalf(sq_dst, side) && squares[sq_dst] == 0)){
			continue;
		}
		sq_dst += BODYGUARD_DELTA[i];
		piece_dst = squares[sq_dst];
		if ((piece_dst & piece_self_side) == 0 || illeagal){
			m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int* CMoveGenerator::GenHorseMoves(int squares[], int sq_src, int side, int illeagal){
	int sq_dst, piece_dst, gen_count = 0;
	const int piece_self_side = CChessUtil::SideTag(side);
	
	for (int i = 0; i < 4; ++i){
		sq_dst = sq_src + KING_DELTA[i];
		if (squares[sq_dst] != 0){
			continue;
		} 
		for (int j = 0; j < 2; ++j){
			sq_dst = sq_src + HORSE_DELTA[i][j];
			if (!CChessUtil::InBoard(sq_dst)){
				continue;
			}
			piece_dst = squares[sq_dst];
			if ((piece_dst & piece_self_side) == 0 || illeagal){
				m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
			}
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int* CMoveGenerator::GenRookMoves(int squares[], int sq_src, int side, int illeagal){
	int sq_dst, piece_dst, gen_count = 0, delta;
	const int piece_opp_side = CChessUtil::OppSideTag(side);
	
	for (int i = 0; i < 4; ++i){
		delta = KING_DELTA[i];
		sq_dst = sq_src + delta;
		while (CChessUtil::InBoard(sq_dst)){
			piece_dst = squares[sq_dst];
			if (piece_dst == 0){
				m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
			}else{
				if (((piece_dst & piece_opp_side) != 0) || illeagal){
					m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
				}
				break;
			}
			sq_dst += delta;
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int* CMoveGenerator::GenCannonMoves(int squares[], int sq_src, int side, int illeagal){
	int sq_dst, piece_dst, gen_count = 0, delta;
	const int piece_opp_side = CChessUtil::OppSideTag(side);
	
	for (int i = 0; i < 4; ++i){
		delta = KING_DELTA[i];
		sq_dst = sq_src + delta;
		while (CChessUtil::InBoard(sq_dst)){
			piece_dst = squares[sq_dst];
			if (piece_dst == 0){
				m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
			}else{
				break;
			}
			sq_dst += delta;
		}
		
		sq_dst += delta;
		while (CChessUtil::InBoard(sq_dst)){
			piece_dst = squares[sq_dst];
			if (piece_dst != 0){
				if ((piece_dst & piece_opp_side) != 0 || illeagal){
					m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
				}
				break;
			}
			sq_dst += delta;
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int* CMoveGenerator::GenPawnMoves(int squares[], int sq_src, int side, int illeagal){
	int sq_dst, piece_dst, gen_count = 0, delta;
	int piece_self_side = CChessUtil::SideTag(side);
	
	sq_dst = CChessUtil::SquareForward(sq_src, side);
	if (CChessUtil::InBoard(sq_dst)){
		piece_dst = squares[sq_dst];
		if ((piece_dst & piece_self_side) == 0 || illeagal){
			m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
		}
	}
	if (CChessUtil::AwayHalf(sq_src, side)){
		for (delta = -1; delta <= 1; delta += 2){
			sq_dst = sq_src + delta;
			if (CChessUtil::InBoard(sq_dst)){
				piece_dst = squares[sq_dst];
				if ((piece_dst & piece_self_side) == 0 || illeagal){
					m_moves[gen_count++] = CChessUtil::Move(sq_src, sq_dst);
				}
			}
		}
	}
	
	m_moves[gen_count] = -1;
	return m_moves;
}

int CMoveGenerator::GenKingFace(int squares[], int sq_src){
	int king_position;
	for (king_position = 0; king_position < BOARD_NUMBER; ++king_position){
		if (CChessUtil::InFort(king_position) && squares[king_position] != 0 && king_position != sq_src){
			int piece = squares[king_position];
			int type = piece & 0x7; 
			if (type == PIECE_KING){
				int mv = CChessUtil::Move(sq_src, king_position);
				if (LegalKingFace(squares, mv)){
					return mv;
				}
				return -1;
			}
		}
	}
	return -1;
}

bool CMoveGenerator::LegalMove(int squares[], int mv, int side){
	int sq_src, sq_dst, sq_pin;
	int piece_self_side, piece_src, piece_dst, delta;
	
	sq_src = CChessUtil::Src(mv);
	piece_src = squares[sq_src];
	piece_self_side = CChessUtil::SideTag(side);
	if ((piece_src & piece_self_side) == 0){
		return false;
	}

	sq_dst = CChessUtil::Dst(mv);
	piece_dst = squares[sq_dst];
	if ((piece_dst & piece_self_side) != 0){
		return false;
	}

	switch (piece_src - piece_self_side){
	case PIECE_KING:
		return CChessUtil::InFort(sq_dst) && CChessUtil::KingSpan(sq_src, sq_dst) || LegalKingFace(squares, mv);
	case PIECE_BODYGUARD:
		return CChessUtil::InFort(sq_dst) && CChessUtil::BodyguardSpan(sq_src, sq_dst);
	case PIECE_BISHOP:
		return CChessUtil::SameHalf(sq_src, sq_dst) && CChessUtil::BishopSpan(sq_src, sq_dst) && squares[CChessUtil::BishopPin(sq_src, sq_dst)] == 0;
	case PIECE_HORSE:
		sq_pin = CChessUtil::HorsePin(sq_src, sq_dst);
		return sq_pin != sq_src && squares[sq_pin] == 0;
	case PIECE_ROOK:
	case PIECE_CANNON:
		if (CChessUtil::SameRank(sq_src, sq_dst)){
			delta = (sq_dst < sq_src ? -1 : 1);
		}else if (CChessUtil::SameFile(sq_src, sq_dst)){
			delta = (sq_dst < sq_src ? -16 : 16);
		}else{
			return false;
		}
		sq_pin = sq_src + delta;
		while (sq_pin != sq_dst && squares[sq_pin] == 0){
			sq_pin += delta;
		}
		if (sq_pin == sq_dst){
			return piece_dst == 0 || piece_src - piece_self_side == PIECE_ROOK;
		}else if (piece_dst != 0 && piece_src - piece_self_side == PIECE_CANNON){
			sq_pin += delta;
			while (sq_pin != sq_dst && squares[sq_pin] == 0){
				sq_pin += delta;
			}
			return sq_pin == sq_dst;
		}else{
			return false;
		}
	case PIECE_PAWN:
		if (CChessUtil::AwayHalf(sq_dst, side) && (sq_dst == sq_src - 1 || sq_dst == sq_src + 1)){
			return true;
		}
		return sq_dst == CChessUtil::SquareForward(sq_src, side);
	default:
		return false;
	}
}