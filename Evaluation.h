#ifndef EVALUATION_H
#define EVALUATION_H
/*
Created on 2015-09-03

@author:jeff
*/

#include "constant.h"

class CEvaluator
{
protected:
	const static int BASEVALUE_PAWN = 100;
	const static int BASEVALUE_BODYGUARD = 250;
	const static int BASEVALUE_BISHOP = 250;
	const static int BASEVALUE_ROOK = 500;
	const static int BASEVALUE_HORSE = 350;
	const static int BASEVALUE_CANNON = 350;
	const static int BASEVALUE_KING = 10000;

	const static int FLEXIBILITY_PAWN = 15;
	const static int FLEXIBILITY_BODYGUARD = 1;
	const static int FLEXIBILITY_BISHOP = 1;
	const static int FLEXIBILITY_ROOK = 6;
	const static int FLEXIBILITY_HORSE = 12;
	const static int FLEXIBILITY_CANNON = 6;
	const static int FLEXIBILITY_KING = 0;

	int m_base_value[PIECE_COUNT];
	int m_flex_value[PIECE_COUNT];
	short m_attack_pos[BOARD_NUMBER];
	short m_guard_pos[BOARD_NUMBER];
	short m_flexibility_pos[BOARD_NUMBER];
	int m_chess_value[BOARD_NUMBER];
	int m_pos_count;
	int m_relate_pos[SINGLE_GEN_MOVE_SIZE];

	int ProtectAttack(int squares[], bool is_red_turn, int depth);
	void GetFlexibility(int squares[]);
	int GetChessValues(int squares[], bool is_red_turn, int depth);
	int GetScore(int squares[], bool is_red_turn);
	int GetRelatePiece(int squares[], int sq_src, int side);
	inline void AddPoint(int mv);
	void AddPoints(int mvs[]);

public:
	CEvaluator();
	virtual ~CEvaluator();
	virtual int Evaluate(int squares[], bool is_red_turn, int depth);

	static int GetPosValue(int sq, int pc);
};
#endif