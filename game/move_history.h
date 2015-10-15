
#ifndef MOVE_HISTORY
#define MOVE_HISTORY

/*
Created on 2015-10-11

@author: jeff
*/

#include <stack>
#include "../logic/chess_move.h"
#include <cstddef>

class CMoveHistory
{
private:
	std::stack<CHESS_MOVE> moves;

public:
	CMoveHistory();
	virtual ~CMoveHistory();
	void PushMove(CHESS_MOVE move);
	void PopMove();
	void Clean();
	CHESS_MOVE TopMove();
	bool IsEmpty();
};

CMoveHistory* GetMoveHistoryInstance();

#endif
