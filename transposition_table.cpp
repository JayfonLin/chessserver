/*
Created on 2015-09-05

@author:jeff
*/

#include "transposition_table.h"
#include "chess_util.h"

#include <cstdlib>
#include <ctime>

LONGLONG Rand64(){
	return rand() ^ ((LONGLONG)rand() << 15) ^ ((LONGLONG)rand() << 30) ^ 
		((LONGLONG)rand() << 45) ^ ((LONGLONG)rand() << 60);
}

long Rand32(){
	return rand() ^ ((long)rand() << 15) ^ ((long)rand() << 30);
}


CTranspositionTable::CTranspositionTable(){
	InitializeHashKey();
}

CTranspositionTable::~CTranspositionTable(){
	delete m_ptt[0];
	delete m_ptt[1];
}

void CTranspositionTable::InitializeHashKey(){
	int i,j,k;
	
	srand((unsigned)time(NULL));
	
	for (i = 0; i < PIECE_COUNT; ++i)
		for (j = 0; j < BOARD_NUMBER; ++j){
			if (!CChessUtil::InBoard(j))
				continue;

			m_hash_key_32[i][j] = Rand32();
			m_hash_key_64[i][j] = Rand64();
		}

	m_ptt[0] = new HashItem[HASH_SIZE];
	m_ptt[1] = new HashItem[HASH_SIZE];
}

void CTranspositionTable::CalculateInitHashKey(int cur_position[]){
	int j, chess_type;
	m_hash_32 = 0;
	m_hash_64 = 0;
	
	for (j = 0; j < BOARD_NUMBER; ++j){
		if (!CChessUtil::InBoard(j)){
			continue;
		}
		
		chess_type = cur_position[j];
		if (chess_type != 0)
		{
			m_hash_32 = m_hash_32 ^ m_hash_key_32[chess_type][j]; 
			m_hash_64 = m_hash_64 ^ m_hash_key_64[chess_type][j]; 
		}
	}
}

void CTranspositionTable::HashMakeMove(CHESS_MOVE move, int cur_position[]){
	int to_id, from_id;
	
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	from_id = cur_position[sq_src];
	to_id = cur_position[sq_dst];

	m_hash_32 = m_hash_32 ^ m_hash_key_32[from_id][sq_src]; 
	m_hash_64 = m_hash_64 ^ m_hash_key_64[from_id][sq_src]; 

	if (to_id != 0){
		m_hash_32 = m_hash_32 ^ m_hash_key_32[to_id][sq_dst]; 
		m_hash_64 = m_hash_64 ^ m_hash_key_64[to_id][sq_dst]; 
	}
	
	m_hash_32 = m_hash_32 ^ m_hash_key_32[from_id][sq_dst]; 
	m_hash_64 = m_hash_64 ^ m_hash_key_64[from_id][sq_dst]; 
}

void CTranspositionTable::HashUnmakeMove(CHESS_MOVE move, int chess_id, int cur_position[]){
	int to_id;
		
	int sq_src = CChessUtil::Src(move.m_move);
	int sq_dst = CChessUtil::Dst(move.m_move);
	to_id = cur_position[sq_dst];
	
	m_hash_32 = m_hash_32 ^ m_hash_key_32[to_id][sq_src]; 
	m_hash_64 = m_hash_64 ^ m_hash_key_64[to_id][sq_src]; 

	m_hash_32 = m_hash_32 ^ m_hash_key_32[to_id][sq_dst]; 
	m_hash_64 = m_hash_64 ^ m_hash_key_64[to_id][sq_dst]; 
	
	if (chess_id != 0){
		m_hash_32 = m_hash_32 ^ m_hash_key_32[chess_id][sq_dst]; 
		m_hash_64 = m_hash_64 ^ m_hash_key_64[chess_id][sq_dst]; 
	}
}

int CTranspositionTable::LookupHashTable(int alpha, int beta, int depth, int table_no){
	int x;
	HashItem *pht;

	x = m_hash_32 & 0xFFFF;
	pht = &m_ptt[table_no][x];

	/*if (pht == null){
		return Constant.INVALID_SCORE;
	}*/
	
    if (pht->depth >= depth && pht->checksum == m_hash_64){
		switch (pht->entry_type) {
		case EXACT: 
			return pht->eval;
		case LOWER_BOUND:
			if (pht->eval >= beta) 
				return (pht->eval);
			else 
				break;
		case UPPER_BOUND:
			if (pht->eval <= alpha) 
				return (pht->eval);
			else 
				break;
        }
	}

	return INVALID_SCORE;
}
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int table_no){
	int x;
	HashItem *pht;

	x = m_hash_32 & 0xFFFF;
	pht = &m_ptt[table_no][x];

	pht->checksum = m_hash_64;
	pht->entry_type = entry_type;
	pht->eval = eval;
	pht->depth = depth;
}