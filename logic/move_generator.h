#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H
/*
Created on 2015-09-03

@author:jeff
*/

#include "constant.h"

class CMoveGenerator
{
protected:
	int m_gen_moves;
	static int m_moves[];
	static bool LegalKingFace(int squares[], int mv);

public:
	int CreatePossibleMove(int squares[], int mvs[], int ply, int side);
	static int* GenKingMoves(int squares[], int sq_src, int side, bool illeagal);
	static int* GenBodyguardMoves(int squares[], int sq_src, int side, bool illeagal);
	static int* GenBishopMoves(int squares[], int sq_src, int side, int illeagal);
	static int* GenHorseMoves(int squares[], int sq_src, int side, int illeagal);
	static int* GenRookMoves(int squares[], int sq_src, int side, int illeagal);
	static int* GenCannonMoves(int squares[], int sq_src, int side, int illeagal);
	static int* GenPawnMoves(int squares[], int sq_src, int side, int illeagal);
	static int GenKingFace(int squares[], int sq_src);
	static bool LegalMove(int squares[], int mv, int side);
	CMoveGenerator();
	~CMoveGenerator();

};

#endif
 