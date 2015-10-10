
/*
Created on 2015-10-03

@author: jeff
*/

#include "handler.h"

typedef void (*handler_func_ptr)(int, struct bufferevent*, CBinUnpacker*);
typedef std::map<int, handler_func_ptr> handler_map;

static handler_map HANDLERS = {
	{CMD_TEST_CONNECTION, OnTestConnection},
	{CMD_CHESS_START_UP, OnStartChessGame},
	{CMD_CHESS_MOVE, OnChessMove}
};

void OnTestConnection(int uid, struct bufferevent *bev, CBinUnpacker *unpacker){
	BYTE b = unpacker->UnpackByte();
	WORD w = unpacker->UnpackWord();
	DWORD d = unpacker->UnpackDWord();
	STRING str = unpacker->UnpackString();

	printf("recv byte: %d\n", b);
	printf("recv word: %x\n", w);
	printf("recv dword: %x\n", d);
	printf("recv string: %s\n", str);
}

void OnPackage(struct bufferevent *bev, CBinUnpacker *unpacker){
	
	int cmd = unpacker->UnpackDWord();
	printf("OnPackage cmd: %x\n", cmd);

	(*HANDLERS.at(cmd))(0, bev, unpacker);
}

