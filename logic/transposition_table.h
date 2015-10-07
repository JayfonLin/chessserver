#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H
/*
Created on 2015-09-05

@author:jeff
*/

#include "constant.h"
#include "chess_move.h"
#include "../define.h"

enum ENTRY_TYPE {EXACT, LOWER_BOUND, UPPER_BOUND};
struct HashItem{
	LONGLONG checksum;
	ENTRY_TYPE entry_type;
	short depth;
	short eval;
};


class CTranspositionTable
{
protected:
	const static int HASH_SIZE = 1024 * (2 << 10);

public:
	
	uint m_hash_key_32[PIECE_COUNT][BOARD_NUMBER];
	ULONGLONG m_hash_key_64[PIECE_COUNT][BOARD_NUMBER];
	HashItem* m_ptt[2];
	uint m_hash_32;
	ULONGLONG m_hash_64;

	CTranspositionTable();
	virtual ~CTranspositionTable();

	void InitializeHashKey();
	void CalculateInitHashKey(int cur_position[]);
	void HashMakeMove(CHESS_MOVE move, int cur_position[]);
	void HashUnmakeMove(CHESS_MOVE move, int chess_id, int cur_position[]);
	int LookupHashTable(int alpha, int beta, int depth, int table_no);
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int table_no);
};

#endif