
/*
Created on 2015-10-03

@author: jeff
*/

#include "handler.h"
#include "../util/commons.h"

typedef void (*handler_func_ptr)(int, struct bufferevent*, CBinUnpacker*);
typedef std::map<int, handler_func_ptr> handler_map;

static handler_map HANDLERS = {
	{CMD_TEST_CONNECTION, OnTestConnection},
	{CMD_CHESS_START_UP, OnStartChessGame},
	{CMD_CHESS_MOVE, OnChessMove},
	{CMD_UNMAKE_MOVE, OnUnmakeMove},
};

void OnTestConnection(int uid, struct bufferevent *bev, CBinUnpacker *unpacker){
	BYTE b = unpacker->UnpackByte();
	WORD w = unpacker->UnpackWord();
	DWORD d = unpacker->UnpackDWord();
	STRING str = unpacker->UnpackString();

	Log("recv byte: %d", b);
	Log("recv word: %x", w);
	Log("recv dword: %x", d);
	Log("recv string: %s", str);
}

void OnPackage(struct bufferevent *bev, CBinUnpacker *unpacker){
	
	int cmd = unpacker->UnpackDWord();
	Log("OnPackage cmd: %x", cmd);

	(*HANDLERS.at(cmd))(0, bev, unpacker);
}

