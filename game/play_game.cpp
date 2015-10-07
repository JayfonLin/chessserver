
/*
Created on 2015-10-05

@author: jeff
*/

#include "play_game.h"
#include <cstdio>

void OnStartChessGame(int uid, struct bufferevent *bev, CBinUnpacker *unpacker){
	printf("OnStartChessGame\n");
	GetBoardInstance();
	CNegaScout_TT_HH* engine = GetEngineInstance();

	int depth = unpacker->UnpackByte();
	engine->SetSearchDepth(depth);
}

void OnChessMove(int uid, struct  bufferevent *bev, CBinUnpacker *unpacker){
	printf("OnChessMove\n");
	CBoard *board = GetBoardInstance();
	int m_move = unpacker->UnpackDWord();
	CHESS_MOVE move;
	move.m_move = m_move;
	board->MakeMove(move);

	SearchGoodMove(uid, bev, board);
}

void SearchGoodMove(int uid, struct bufferevent *bev, CBoard* board){

	CNegaScout_TT_HH* engine = GetEngineInstance();
	int* cur_board = board->GetCurBoard();
	CHESS_MOVE chess_move = engine->SearchAGoodMove(cur_board);
	board->MakeMove(chess_move);

	CBinPacker packet;
	packet.PackDWord(CMD_CHESS_MOVE);
	packet.PackDWord(chess_move.m_move);

	evbuffer* buf = packet.GetPackBuffer();
    evutil_socket_t fd = bufferevent_getfd(bev);
    int result = evbuffer_write(buf, fd);
    if (result <= 0){
        printf("send failed!");
    }

}