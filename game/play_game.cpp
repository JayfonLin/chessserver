
/*
Created on 2015-10-05

@author: jeff
*/

#include "play_game.h"
#include <cstdio>

void OnStartChessGame(int uid, struct bufferevent *bev, CBinUnpacker *unpacker){
	Log("OnStartChessGame");
	CBoard *board = GetBoardInstance();
	board->Initialize();
	CNegaScout_TT_HH* engine = GetEngineInstance();

	int depth = unpacker->UnpackByte();
	engine->SetSearchDepth(depth);


}

void OnChessMove(int uid, struct bufferevent *bev, CBinUnpacker *unpacker){
	Log("OnChessMove");
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

	BYTE is_kill = 0;

	int sq_dst = CChessUtil::Dst(chess_move.m_move);
	if (cur_board[sq_dst] != 0)
		is_kill = 1;

	if (CChessUtil::IsGameOver(cur_board))
		is_kill = 2;

	board->MakeMove(chess_move);

	CBinPacker packet;
	packet.PackDWord(CMD_CHESS_MOVE);
	packet.PackDWord(chess_move.m_move);
	packet.PackByte(is_kill);

	evbuffer* buf = packet.GetPackBuffer();
    evutil_socket_t fd = bufferevent_getfd(bev);
    int result = evbuffer_write(buf, fd);
    if (result <= 0){
        Log("send failed!");
    }

}

void OnUnmakeMove(int uid, struct bufferevent *bev, CBinUnpacker *pack){
	Log("OnUnmakeMove");
	CBoard *board = GetBoardInstance();

    if (!board->CanUnmakeMove())
        return;

    CHESS_MOVE move1 = board->GetLastMove();
    board->UnmakeMove();
	SendUnmakeMove(bev, move1);

	if (!board->CanUnmakeMove())
        return;

    CHESS_MOVE move2 = board->GetLastMove();
    board->UnmakeMove();
	SendUnmakeMove(bev, move2);
};

void SendUnmakeMove(struct bufferevent *bev, CHESS_MOVE move){

	Log("SendUnmakeMove");

	CBinPacker packet;
	packet.PackDWord(CMD_UNMAKE_MOVE);
	packet.PackDWord(move.m_move);
	packet.PackByte(move.m_chess_id);

	evbuffer* buf = packet.GetPackBuffer();
    evutil_socket_t fd = bufferevent_getfd(bev);
    int result = evbuffer_write(buf, fd);
    if (result <= 0){
        Log("send failed!");
    }
}
