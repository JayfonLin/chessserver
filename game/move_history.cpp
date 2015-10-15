
/*
Created on 2015-10-11

@author: jeff
*/

#include "move_history.h"

CMoveHistory::CMoveHistory(){}

CMoveHistory::~CMoveHistory(){}

void CMoveHistory::PushMove(CHESS_MOVE move){
	moves.push(move);
}

void CMoveHistory::PopMove(){
    if (moves.empty()){
        return;
    }

	moves.pop();
}

void CMoveHistory::Clean(){
	while (!moves.empty()){
		moves.pop();
	}
}

CHESS_MOVE CMoveHistory::TopMove(){
    return moves.top();
}

bool CMoveHistory::IsEmpty(){
    return moves.empty();
}

static CMoveHistory* instance = new CMoveHistory();
CMoveHistory* GetMoveHistoryInstance(){
	return instance;
}
